#include "Bus.h"

uint8_t Bus::bus_read(uint16_t address){
    if (address < 0x8000) {
        //ROM Data
        return cart->cart_read(address);
    } else if (address < 0xA000) {
        //Char/Map Data
        return ppu->ppu_vram_read(address);
    } else if (address < 0xC000) {
        //Cartridge RAM
        return cart->cart_read(address);
    } else if (address < 0xE000) {
        //WRAM (Working RAM)
        return ram->wram_read(address);
    } else if (address < 0xFE00) {
        //reserved echo ram...
        return 0;
    } else if (address < 0xFEA0) {
        //OAM
        if (dma->dma_transferring()) {
            return 0xFF;
        }

        return ppu->ppu_oam_read(address);
    } else if (address < 0xFF00) {
        //reserved unusable...
        return 0;
    } else if (address < 0xFF80) {
        //IO Registers...
        return io->io_read(address);
    } else if (address == 0xFFFF) {
        //CPU ENABLE REGISTER...
        return cpu->cpu_get_ie_register();
    }

    return ram->hram_read(address);
}

uint16_t Bus::bus_read16(uint16_t address){
    uint16_t lo = bus_read(address);
    uint16_t hi = bus_read(address + 1);
    return lo | (hi << 8);
}

void Bus::bus_write(uint16_t address, uint8_t value){
    if (address < 0x8000) {
        //ROM Data
        cart->cart_write(address, value);
    } else if (address < 0xA000) {
        //Char/Map Data
        ppu->ppu_vram_write(address, value);
    } else if (address < 0xC000) {
        //EXT-RAM
        cart->cart_write(address, value);
    } else if (address < 0xE000) {
        //WRAM
        ram->wram_write(address, value);
    } else if (address < 0xFE00) {
        //reserved echo ram
    } else if (address < 0xFEA0) {
        //OAM
        if (dma->dma_transferring()) {
            return;
        }
        
        ppu->ppu_oam_write(address, value);
    } else if (address < 0xFF00) {
        //unusable reserved
    } else if (address < 0xFF80) {
        //IO Registers...
        io->io_write(address, value);
    } else if (address == 0xFFFF) {
        //CPU SET ENABLE REGISTER
        
        cpu->cpu_set_ie_register(value);
    } else {
        ram->hram_write(address, value);
    }
}

void Bus::bus_write16(uint16_t address, uint16_t value){
    bus_write(address + 1, (value >> 8) & 0xFF);
    bus_write(address, value & 0xFF);
}