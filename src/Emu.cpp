#include "Emu.h"
#include <unistd.h> 
EMU::EMU(int argv,char **argc) : cart(argv<2?nullptr:argc[1])
{
    if(argv<2){
        cout<<"NO ARGUMENTS FOR ROM"<<'\n';
        return;
    }
    link_classes();
    cpu_run();
}

void EMU::link_classes(){
    cpu = new Cpu();
    timer = new Timer();
    bus = new Bus();
    ram = new Ram();

    
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