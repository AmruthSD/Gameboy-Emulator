#include "Lcd.h"

unsigned long colors_default[4] = {0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000};

bool Lcd::LCDC_BGW_ENABLE(){
    return BIT(lcdc,0);
}
bool Lcd::LCDC_OBJ_ENABLE(){
    return BIT(lcdc,1);
}
bool Lcd::LCDC_WIN_ENABLE(){
    return BIT(lcdc,5);
}
bool Lcd::LCDC_LCD_ENABLE(){
    return BIT(lcdc,7);
}
uint16_t Lcd::LCDC_OBJ_HEIGHT(){
    return (BIT(lcdc, 2) ? 16 : 8);
}
uint16_t Lcd::LCDC_BG_MAP_AREA(){
    return (BIT(lcdc, 3) ? 0x9C00 : 0x9800);
}
uint16_t Lcd::LCDC_BGW_DATA_AREA(){
    return (BIT(lcdc, 4) ? 0x8000 : 0x8800);
}
uint16_t Lcd::LCDC_WIN_MAP_AREA(){
    return (BIT(lcdc, 6) ? 0x9C00 : 0x9800);
}

bool Lcd::LCDS_LYC(){
    return (BIT(lcds, 2));
}
void Lcd::LCDS_LYC_SET(bool b){
    return (BIT_SET(lcds, 2, b));
}

void Lcd::lcd_init(){
    lcdc = 0x91;
    scroll_x = 0;
    scroll_y = 0;
    ly = 0;
    ly_compare = 0;
    bg_palette = 0xFC;
    obj_palette[0] = 0xFF;
    obj_palette[1] = 0xFF;
    win_y = 0;
    win_x = 0;

    for (int i=0; i<4; i++) {
        bg_colors[i] = colors_default[i];
        sp1_colors[i] = colors_default[i];
        sp2_colors[i] = colors_default[i];
    }
}

uint8_t Lcd::lcd_read(uint16_t address){
    uint8_t offset = (address - 0xFF40);
    uint8_t *p = (uint8_t *)(this);
    return p[offset];
}

void Lcd::update_palette(uint8_t palette_data, uint8_t pal) {
    uint32_t *p_colors = bg_colors;

    switch(pal) {
        case 1:
            p_colors = sp1_colors;
            break;
        case 2:
            p_colors = sp2_colors;
            break;
    }

    p_colors[0] = colors_default[palette_data & 0b11];
    p_colors[1] = colors_default[(palette_data >> 2) & 0b11];
    p_colors[2] = colors_default[(palette_data >> 4) & 0b11];
    p_colors[3] = colors_default[(palette_data >> 6) & 0b11];
}

void Lcd::lcd_write(uint16_t address, uint8_t value) {

    uint8_t offset = (address - 0xFF40);
    uint8_t *p = (uint8_t *)(this);
    p[offset] = value;

    if (offset == 6) { 
        //0xFF46 = DMA
        //dma_start(value);
    }

    if (address == 0xFF47) {
        update_palette(value, 0);
    } else if (address == 0xFF48) {
        update_palette(value & 0b11111100, 1);
    } else if (address == 0xFF49) {
        update_palette(value & 0b11111100, 2);
    }
}