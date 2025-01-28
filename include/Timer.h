#pragma once

#include <iostream>
#include "Cpu.h"
using namespace std;

class Timer
{

private:
    uint16_t div;
    uint8_t tima;
    uint8_t tma;
    uint8_t tac;
    Cpu *cpu;

public:
    Timer();
    void timer_tick();

    void timer_write(uint16_t address, uint8_t value);
    uint8_t timer_read(uint16_t address);
};
