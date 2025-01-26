#include "Bus.h"

uint8_t Bus::bus_read(uint16_t address){
    if(address < 0x8000){
        //rom read
    }
}

void Bus::bus_write(uint16_t address, uint8_t value){
    if(address < 0x8000){
        //rom write
    }
}

void Bus::bus_write16(uint16_t address, uint16_t value){
    
}