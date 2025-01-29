#pragma once

#include <iostream>
#include "Cart.h"
#include "Ram.h"
#include "Cpu.h"

class Cpu;

class Bus
{
private:
    Cartridge *cart;
    Ram *ram;
    Cpu *cpu;

public:
    uint8_t bus_read(uint16_t address);
    uint16_t bus_read16(uint16_t address);
    void bus_write(uint16_t address, uint8_t value);
    void bus_write16(uint16_t address, uint16_t value);
};

