#pragma once

#include <iostream>
#include "Cart.h"
#include "Cpu.h"
#include "Bus.h"
#include "Timer.h"
#include "Ram.h"
#include "Ui.h"
#include "Ppu.h"
#include "Dma.h"
#include "Io.h"
#include "Lcd.h"
#include "Gamepad.h"

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
    Ui *ui;
    Ppu *ppu;
    Dma *dma;
    Io *io;
    Lcd *lcd;
    Gamepad *gamepad;

public:
    EMU(int argv,char **argc);
    void cpu_run();
    void link_classes();
};

