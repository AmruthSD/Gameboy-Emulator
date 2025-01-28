#ifndef EMU_H
#define EMU_H

#include <iostream>
#include "Cart.h"
#include "Cpu.h"
#include "Bus.h"
#include "Timer.h"
#include "Ram.h"
using namespace std;

class EMU
{
private:
    Cartridge cart;
    bool paused;
    bool running;
    Cpu *cpu;
    Bus *bus;
    Timer *timer;
    Ram *ram;

public:
    EMU(int argv,char **argc);
    void cpu_run();
    void link_classes();
};

#endif