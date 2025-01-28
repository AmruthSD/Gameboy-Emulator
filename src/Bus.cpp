#include "Bus.h"

uint8_t Bus::bus_read(uint16_t address){
    if(address < 0x8000){
        //rom read
        return cart->cart_read(address);
    }
}

uint16_t Bus::bus_read16(uint16_t address){
    uint16_t lo = bus_read(address);
    uint16_t hi = bus_read(address + 1);
    return lo | (hi << 8);
}

void Bus::bus_write(uint16_t address, uint8_t value){
    if(address < 0x8000){
        //rom write
    }
}

void Bus::bus_write16(uint16_t address, uint16_t value){
    bus_write(address + 1, (value >> 8) & 0xFF);
    bus_write(address, value & 0xFF);
}