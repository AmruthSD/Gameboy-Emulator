#ifndef EMU_H
#define EMU_H

#include <iostream>
#include "Cart.h"
#include "Cpu.h"
#include "Bus.h"
using namespace std;

class EMU
{
private:
    Cartridge cart;
    bool paused;
    bool running;
    uint64_t ticks;
    Cpu cpu;
    Bus bus;

public:
    EMU(int argv,char **argc);
    ~EMU();
};

#endif