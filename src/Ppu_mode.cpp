#include "Ppu.h"
#include <cstring>

void Ppu::increment_ly(){
    if (window_visible() && lcd->get_ly() >= lcd->get_win_y() &&
        lcd->get_ly() < lcd->get_win_y() + YRES) {
            window_line++;
    }

    lcd->inc_ly();

    if (lcd->get_ly() == lcd->get_ly_compare()) {
        lcd->LCDS_LYC_SET(1);

        if (lcd->LCDS_STAT_INT(SS_LYC)) {
            cpu->cpu_request_interrupt(IT_LCD_STAT);
        }
    } else {
        lcd->LCDS_LYC_SET(0);
    }
}

void Ppu::load_line_sprites() {
    int cur_y = lcd->get_ly();

    uint8_t sprite_height = lcd->LCDC_OBJ_HEIGHT();
    memset(line_entry_array, 0, sizeof(line_entry_array));

    for (int i=0; i<40; i++) {
        oam_entry e = oam_ram[i];

        if (!e.x) {
            //x = 0 means not visible...
            continue;
        }

        if (line_sprite_count >= 10) {
            //max 10 sprites per line...
            break;
        }

        if (e.y <= cur_y + 16 && e.y + sprite_height > cur_y + 16) {
            //this sprite is on the current line.

            oam_line_entry *entry = &line_entry_array[
                line_sprite_count++
            ];

            entry->entry = e;
            entry->next = NULL;

            if (!line_sprites ||
                    line_sprites->entry.x > e.x) {
                entry->next = line_sprites;
                line_sprites = entry;
                continue;
            }

            //do some sorting...

            oam_line_entry *le = line_sprites;
            oam_line_entry *prev = le;

            while(le) {
                if (le->entry.x > e.x) {
                    prev->next = entry;
                    entry->next = le;
                    break;
                }

                if (!le->next) {
                    le->next = entry;
                    break;
                }

                prev = le;
                le = le->next;
            }
        }
    }
}

void Ppu::ppu_mode_oam(){
    if (line_ticks >= 80) {
        lcd->LCDS_MODE_SET(MODE_XFER);

        pfc.cur_fetch_state = FS_TILE;
        pfc.line_x = 0;
        pfc.fetch_x = 0;
        pfc.pushed_x = 0;
        pfc.fifo_x = 0;
    }

    if (line_ticks == 1) {
        //read oam on the first tick only...
        line_sprites = 0;
        line_sprite_count = 0;
        load_line_sprites();
    }
}

void Ppu::ppu_mode_xfer(){
    
    pipeline_process();
    
    if (pfc.pushed_x >= XRES) {
        pipeline_fifo_reset();

        lcd->LCDS_MODE_SET(MODE_HBLANK);

        if (lcd->LCDS_STAT_INT(SS_HBLANK)) {
            cpu->cpu_request_interrupt(IT_LCD_STAT);
        }
    }
}

void Ppu::ppu_mode_vblank(){
    if (line_ticks >= TICKS_PER_LINE) {
        increment_ly();

        if (lcd->get_ly() >= LINES_PER_FRAME) {
            lcd->LCDS_MODE_SET(MODE_OAM);
            lcd->set_ly(0);
            window_line = 0;
        }
        line_ticks = 0;
    }
}

static uint32_t target_frame_time = 1000 / 60;
static long prev_frame_time = 0;
static long start_timer = 0;
static long frame_count = 0;


void Ppu::ppu_mode_hblank(){
    if (line_ticks >= TICKS_PER_LINE) {
        increment_ly();

        if (lcd->get_ly() >= YRES) {
            lcd->LCDS_MODE_SET(MODE_VBLANK);

            cpu->cpu_request_interrupt(IT_VBLANK);

            if (lcd->LCDS_STAT_INT(SS_VBLANK)) {
                cpu->cpu_request_interrupt(IT_LCD_STAT);
            }

            current_frame++;

            //calc FPS...
            uint32_t end = ui->get_ticks();
            
            uint32_t frame_time = end - prev_frame_time;

            if (frame_time < target_frame_time) {
                ui->delay((target_frame_time - frame_time));
            }

            if (end - start_timer >= 1000) {
                uint32_t fps = frame_count;
                start_timer = end;
                frame_count = 0;

                printf("FPS: %d\n", fps);

                if (cart->cart_need_save()) {
                    cart->cart_battery_save();
                }
            }

            frame_count++;
            prev_frame_time = ui->get_ticks();

        } else {
            lcd->LCDS_MODE_SET(MODE_OAM);
        }
        line_ticks = 0;
    }
}
