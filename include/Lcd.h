#pragma once
#include "Common.h"


#define LCDS_MODE ((lcd_mode)(lcds & 0b11))
#define LCDS_MODE_SET(mode) { lcds &= ~0b11; lcds |= mode; }
#define LCDS_STAT_INT(src) (lcds & src)

typedef enum {
    MODE_HBLANK,
    MODE_VBLANK,
    MODE_OAM,
    MODE_XFER
} lcd_mode;

typedef enum {
    SS_HBLANK = (1 << 3),
    SS_VBLANK = (1 << 4),
    SS_OAM = (1 << 5),
    SS_LYC = (1 << 6),
} stat_src;

class Lcd
{
private:
    uint8_t lcdc;
    uint8_t lcds;
    uint8_t scroll_y;
    uint8_t scroll_x;
    uint8_t ly;
    uint8_t ly_compare;
    uint8_t dma;
    uint8_t bg_palette;
    uint8_t obj_palette[2];
    uint8_t win_y;
    uint8_t win_x;

    uint32_t bg_colors[4];
    uint32_t sp1_colors[4];
    uint32_t sp2_colors[4];

public:
    void lcd_init();
    uint8_t lcd_read(uint16_t address);
    void lcd_write(uint16_t address, uint8_t value);
    void update_palette(uint8_t palette_data, uint8_t pal);

    bool LCDC_BGW_ENABLE();
    bool LCDC_OBJ_ENABLE();
    bool LCDC_WIN_ENABLE();
    bool LCDC_LCD_ENABLE();
    uint16_t LCDC_OBJ_HEIGHT();
    uint16_t LCDC_BG_MAP_AREA();
    uint16_t LCDC_BGW_DATA_AREA();
    uint16_t LCDC_WIN_MAP_AREA();
    
    bool LCDS_LYC();
    void LCDS_LYC_SET(bool b);
};
