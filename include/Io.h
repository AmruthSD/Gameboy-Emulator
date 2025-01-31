#pragma once

#include "Common.h"
#include "Dma.h"
#include "Lcd.h"
#include "Timer.h"
#include "Cpu.h"
#include "Gamepad.h"

class Dma;
class Lcd;
class Timer;
class Cpu;
class Gamepad;

class Io
{
private:
    char serial_data[2];
    Dma *dma;
    Lcd *lcd;
    Timer *timer;
    Cpu *cpu;
    Gamepad *gamepad;
    
public:
    uint8_t io_read(uint16_t address);
    void io_write(uint16_t address, uint8_t value);
};


