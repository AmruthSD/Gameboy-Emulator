#pragma once

#include <iostream>
#include "Cart.h"
#include "Ram.h"
#include "Cpu.h"
#include "Ppu.h"
#include "Dma.h"
#include "Io.h"

class Cpu;
class Ram;
class Cartridge;
class Ppu;
class Dma;
class Io;

class Bus
{
public:
    Cartridge *cart;
    Ram *ram;
    Cpu *cpu;
    Ppu *ppu;
    Dma *dma;
    Io *io;
    
public:
    uint8_t bus_read(uint16_t address);
    uint16_t bus_read16(uint16_t address);
    void bus_write(uint16_t address, uint8_t value);
    void bus_write16(uint16_t address, uint16_t value);
    void dbg_update();
    void dbg_print();
};

