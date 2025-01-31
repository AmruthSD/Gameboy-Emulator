#include "Ppu.h"

bool Ppu::window_visible() {
    return lcd->LCDC_WIN_ENABLE() && lcd->get_win_x() >= 0 &&
        lcd->get_win_x() <= 166 && lcd->get_win_y() >= 0 &&
        lcd->get_win_y() < YRES;
}

void Ppu::pixel_fifo_push(uint32_t value) {
    fifo_entry *next = (fifo_entry *)malloc(sizeof(fifo_entry));
    next->next = NULL;
    next->value = value;

    if (!pfc.pixel_fifo.head) {
        //first entry...
        pfc.pixel_fifo.head = pfc.pixel_fifo.tail = next;
    } else {
        pfc.pixel_fifo.tail->next = next;
        pfc.pixel_fifo.tail = next;
    }
    pfc.pixel_fifo.size++;
}

uint32_t Ppu::pixel_fifo_pop() {
    if (pfc.pixel_fifo.size <= 0) {
        exit(-8);
    }

    fifo_entry *popped = pfc.pixel_fifo.head;
    pfc.pixel_fifo.head = popped->next;
    pfc.pixel_fifo.size--;

    uint32_t val = popped->value;
    free(popped);

    return val;
}

uint32_t Ppu::fetch_sprite_pixels(int bit, uint32_t color, uint8_t bg_color) {
    for (int i=0; i<fetched_entry_count; i++) {
        int sp_x = (fetched_entries[i].x - 8) + 
            ((lcd->get_scroll_x() % 8));
        
        if (sp_x + 8 < pfc.fifo_x) {
            //past pixel point already...
            continue;
        }

        int offset = pfc.fifo_x - sp_x;

        if (offset < 0 || offset > 7) {
            //out of bounds..
            continue;
        }

        bit = (7 - offset);

        if (fetched_entries[i].f_x_flip) {
            bit = offset;
        }

        uint8_t hi = !!(pfc.fetch_entry_data[i * 2] & (1 << bit));
        uint8_t lo = !!(pfc.fetch_entry_data[(i * 2) + 1] & (1 << bit)) << 1;

        bool bg_priority = fetched_entries[i].f_bgp;

        if (!(hi|lo)) {
            //transparent
            continue;
        }

        if (!bg_priority || bg_color == 0) {
            color = (fetched_entries[i].f_pn) ? 
                lcd->get_sp2_colors()[hi|lo] : lcd->get_sp1_colors()[hi|lo];

            if (hi|lo) {
                break;
            }
        }
    }
    return color;
}

bool Ppu::pipeline_fifo_add() {
    if (pfc.pixel_fifo.size > 8) {
        //fifo is full!
        return false;
    }

    int x = pfc.fetch_x - (8 - (lcd->get_scroll_x() % 8));

    for (int i=0; i<8; i++) {
        int bit = 7 - i;
        uint8_t hi = !!(pfc.bgw_fetch_data[1] & (1 << bit));
        uint8_t lo = !!(pfc.bgw_fetch_data[2] & (1 << bit)) << 1;
        uint32_t color = lcd->get_bg_colors()[hi | lo];

        if (!lcd->LCDC_BGW_ENABLE()) {
            color = lcd->get_bg_colors()[0];
        }

        if (lcd->LCDC_OBJ_ENABLE()) {
            color = fetch_sprite_pixels(bit, color, hi | lo);
        }

        if (x >= 0) {
            pixel_fifo_push(color);
            pfc.fifo_x++;
        }
    }

    return true;
}

void Ppu::pipeline_load_sprite_tile() {
    oam_line_entry *le = line_sprites;

    while(le) {
        int sp_x = (le->entry.x - 8) + (lcd->get_scroll_x() % 8);

        if ((sp_x >= pfc.fetch_x && sp_x < pfc.fetch_x + 8) ||
            ((sp_x + 8) >= pfc.fetch_x && (sp_x + 8) < pfc.fetch_x + 8)) {
            //need to add entry
            fetched_entries[fetched_entry_count++] = le->entry;
        }

        le = le->next;

        if (!le || fetched_entry_count >= 3) {
            //max checking 3 sprites on pixels
            break;
        }
    }
}

void Ppu::pipeline_load_sprite_data(uint8_t offset) {
    int cur_y = lcd->get_ly();
    uint8_t sprite_height = lcd->LCDC_OBJ_HEIGHT();

    for (int i=0; i<fetched_entry_count; i++) {
        uint8_t ty = ((cur_y + 16) - fetched_entries[i].y) * 2;

        if (fetched_entries[i].f_y_flip) {
            //flipped upside down...
            ty = ((sprite_height * 2) - 2) - ty;
        }

        uint8_t tile_index = fetched_entries[i].tile;

        if (sprite_height == 16) {
            tile_index &= ~(1); //remove last bit...
        }

        pfc.fetch_entry_data[(i * 2) + offset] = 
            bus->bus_read(0x8000 + (tile_index * 16) + ty + offset);
    }
}

void Ppu::pipeline_load_window_tile() {
    if (!window_visible()) {
        return;
    }
    
    uint8_t window_y = lcd->get_win_y();

    if (pfc.fetch_x + 7 >= lcd->get_win_x() &&
            pfc.fetch_x + 7 < lcd->get_win_x() + YRES + 14) {
        if (lcd->get_ly() >= window_y && lcd->get_ly() < window_y + XRES) {
            uint8_t w_tile_y = window_line / 8;

            pfc.bgw_fetch_data[0] = bus->bus_read(lcd->LCDC_WIN_MAP_AREA() + 
                ((pfc.fetch_x + 7 - lcd->get_win_x()) / 8) +
                (w_tile_y * 32));

            if (lcd->LCDC_BGW_DATA_AREA() == 0x8800) {
                pfc.bgw_fetch_data[0] += 128;
            }
        }
    }
}

void Ppu::pipeline_fetch() {
    switch(pfc.cur_fetch_state) {
        case FS_TILE: {
            fetched_entry_count = 0;

            if (lcd->LCDC_BGW_ENABLE()) {
                pfc.bgw_fetch_data[0] = bus->bus_read(lcd->LCDC_BG_MAP_AREA() + 
                    (pfc.map_x / 8) + 
                    (((pfc.map_y / 8)) * 32));
            
                if (lcd->LCDC_BGW_DATA_AREA() == 0x8800) {
                    pfc.bgw_fetch_data[0] += 128;
                }

                pipeline_load_window_tile();
            }

            if (lcd->LCDC_OBJ_ENABLE() && line_sprites) {
                pipeline_load_sprite_tile();
            }

            pfc.cur_fetch_state = FS_DATA0;
            pfc.fetch_x += 8;
        } break;

        case FS_DATA0: {
            pfc.bgw_fetch_data[1] = bus->bus_read(lcd->LCDC_BGW_DATA_AREA() +
                (pfc.bgw_fetch_data[0] * 16) + 
                pfc.tile_y);

            pipeline_load_sprite_data(0);

            pfc.cur_fetch_state = FS_DATA1;
        } break;

        case FS_DATA1: {
            pfc.bgw_fetch_data[2] = bus->bus_read(lcd->LCDC_BGW_DATA_AREA() +
                (pfc.bgw_fetch_data[0] * 16) + 
                pfc.tile_y + 1);

            pipeline_load_sprite_data(1);

            pfc.cur_fetch_state = FS_IDLE;

        } break;

        case FS_IDLE: {
            pfc.cur_fetch_state = FS_PUSH;
        } break;

        case FS_PUSH: {
            if (pipeline_fifo_add()) {
                pfc.cur_fetch_state = FS_TILE;
            }

        } break;

    }
}

void Ppu::pipeline_push_pixel() {
    if (pfc.pixel_fifo.size > 8) {
        uint32_t pixel_data = pixel_fifo_pop();

        if (pfc.line_x >= (lcd->get_scroll_x() % 8)) {
            video_buffer[pfc.pushed_x + 
                (lcd->get_ly() * XRES)] = pixel_data;

            pfc.pushed_x++;
        }

        pfc.line_x++;
    }
}

void Ppu::pipeline_process() {
    pfc.map_y = (lcd->get_ly() + lcd->get_scroll_y());
    pfc.map_x = (pfc.fetch_x + lcd->get_scroll_x());
    pfc.tile_y = ((lcd->get_ly() + lcd->get_scroll_y()) % 8) * 2;

    if (!(line_ticks & 1)) {
        pipeline_fetch();
    }

    pipeline_push_pixel();
}

void Ppu::pipeline_fifo_reset() {
    while(pfc.pixel_fifo.size) {
        pixel_fifo_pop();
    }

    pfc.pixel_fifo.head = 0;
}