#pragma once

#include <iostream>
#include "Lcd.h"
#include "Cpu.h"
#include "Cart.h"
#include "Bus.h"
#include "Ui.h"

static const int LINES_PER_FRAME = 154;
static const int TICKS_PER_LINE = 456;
static const int YRES = 144;
static const int XRES = 160;

typedef enum {
    FS_TILE,
    FS_DATA0,
    FS_DATA1,
    FS_IDLE,
    FS_PUSH
} fetch_state;

typedef struct _fifo_entry {
    struct _fifo_entry *next;
    uint32_t value; //32 bit color value.
} fifo_entry;

typedef struct {
    fifo_entry *head;
    fifo_entry *tail;
    uint32_t size;
} fifo;

typedef struct {
    fetch_state cur_fetch_state;
    fifo pixel_fifo;
    uint8_t line_x;
    uint8_t pushed_x;
    uint8_t fetch_x;
    uint8_t bgw_fetch_data[3];
    uint8_t fetch_entry_data[6]; //oam data..
    uint8_t map_y;
    uint8_t map_x;
    uint8_t tile_y;
    uint8_t fifo_x;
} pixel_fifo_context;

typedef struct {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    
    uint8_t f_cgb_pn : 3;
    uint8_t f_cgb_vram_bank : 1;
    uint8_t f_pn : 1;
    uint8_t f_x_flip : 1;
    uint8_t f_y_flip : 1;
    uint8_t f_bgp : 1;

} oam_entry;

typedef struct _oam_line_entry {
    oam_entry entry;
    struct _oam_line_entry *next;
} oam_line_entry;

class Lcd;
class Cpu;
class Cartridge;
class Bus;
class Ui;

class Ppu
{
private:
    oam_entry oam_ram[40];
    uint8_t vram[0x2000];
    pixel_fifo_context pfc;
    
    uint8_t line_sprite_count;
    oam_line_entry *line_sprites;
    oam_line_entry line_entry_array[10];

    uint8_t fetched_entry_count;
    oam_entry fetched_entries[3];
    uint8_t window_line;

    uint32_t current_frame;
    uint32_t line_ticks;
    uint32_t *video_buffer;

public:
    Lcd *lcd;
    Cpu *cpu;
    Cartridge *cart;
    Bus *bus;
    Ui *ui;
    
public:
    void ppu_init();
    void ppu_tick();

    void ppu_oam_write(uint16_t address, uint8_t value);
    uint8_t ppu_oam_read(uint16_t address);

    void ppu_vram_write(uint16_t address, uint8_t value);
    uint8_t ppu_vram_read(uint16_t address); 

    void ppu_mode_oam();
    void ppu_mode_xfer();
    void ppu_mode_vblank();
    void ppu_mode_hblank();

    void load_line_sprites();
    void increment_ly();

    bool window_visible();
    
    void pixel_fifo_push(uint32_t value);
    uint32_t pixel_fifo_pop();
    uint32_t fetch_sprite_pixels(int bit, uint32_t color, uint8_t bg_color);
    bool pipeline_fifo_add();
    void pipeline_load_sprite_tile();
    void pipeline_load_sprite_data(uint8_t offset);
    void pipeline_load_window_tile();
    void pipeline_fetch();
    void pipeline_push_pixel();
    void pipeline_process();
    void pipeline_fifo_reset();

    uint32_t *get_video_buffer();
};