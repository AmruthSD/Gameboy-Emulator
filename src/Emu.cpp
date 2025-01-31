#include "Emu.h"
#include <unistd.h> 
#include <thread>
#include <functional> 
EMU::EMU(int argv,char **argc) : cart(argv<2?nullptr:argc[1])
{
    if(argv<2){
        cout<<"NO ARGUMENTS FOR ROM"<<'\n';
        return;
    }
    link_classes();
    std::thread cpu_thread(std::bind(&EMU::cpu_run, this));
    cout<<"Starting"<<'\n';
    cout<<flush;
    
    ui->ui_init();
    ui->ui_handle_events();
    
    cpu_thread.join();
}

void EMU::link_classes(){
    cpu = new Cpu();
    timer = new Timer();
    bus = new Bus();
    ram = new Ram();
    ui = new Ui();
    ppu = new Ppu();
    dma = new Dma();
    io = new Io();
    lcd = new Lcd();
    gamepad = new Gamepad();

    

    bus->cart = &cart;
    bus->ram = ram;
    bus->cpu = cpu;
    bus->dma = dma;
    bus->io = io;
    bus->ppu = ppu;

    cpu->bus = bus;
    cpu->timer = timer;
    cpu->dma = dma;
    cpu->ppu = ppu;

    io->dma = dma;
    io->cpu = cpu;
    io->gamepad = gamepad;
    io->lcd = lcd;
    io->timer = timer;

    lcd->DMA = dma;

    ppu->bus = bus;
    ppu->cart = &cart;
    ppu->cpu = cpu;
    ppu->lcd = lcd;
    ppu->ui = ui;

    timer->cpu = cpu;

    ui->bus = bus;
    ui->ppu = ppu;
    ui->gamepad = gamepad;

    dma->ppu = ppu;
    dma->bus = bus;

    ppu->ppu_init();
}


void EMU::cpu_run(){
    paused = false;
    running = true;
    while (running)
    {
        if(paused){
            sleep(1);
        }
        
        if(!cpu->cpu_step()){
            cout<<"CPU STOPPED"<<'\n';
            return;
        }
    }
}