#include "Ppu.h"
#include <cstring>

void Ppu::ppu_init(){
    current_frame = 0;
    line_ticks = 0;
    video_buffer = (uint32_t *)malloc(YRES * XRES * sizeof(uint32_t));

    pfc.line_x = 0;
    pfc.pushed_x = 0;
    pfc.fetch_x = 0;
    pfc.pixel_fifo.size = 0;
    pfc.pixel_fifo.head = pfc.pixel_fifo.tail = NULL;
    pfc.cur_fetch_state = FS_TILE;

    line_sprites = 0;
    fetched_entry_count = 0;
    window_line = 0;

    lcd->lcd_init();
    lcd->LCDS_MODE_SET(MODE_OAM);
    memset(oam_ram, 0, sizeof(oam_ram));
    memset(video_buffer, 0, YRES * XRES * sizeof(uint32_t));
}

void Ppu::ppu_tick(){
    line_ticks++;
    switch(lcd->LCDS_MODE()) {
    case MODE_OAM:
        ppu_mode_oam();
        break;
    case MODE_XFER:
        ppu_mode_xfer();
        break;
    case MODE_VBLANK:
        ppu_mode_vblank();
        break;
    case MODE_HBLANK:
        ppu_mode_hblank();
        break;
    }
    
}


void Ppu::ppu_oam_write(uint16_t address, uint8_t value){
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)oam_ram;
    p[address] = value;
}

uint8_t Ppu::ppu_oam_read(uint16_t address){
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)oam_ram;
    return p[address];
}


void Ppu::ppu_vram_write(uint16_t address, uint8_t value){
    vram[address - 0x8000] = value;
}

uint8_t Ppu::ppu_vram_read(uint16_t address){
    return vram[address - 0x8000];
} 
