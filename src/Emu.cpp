#include "Emu.h"
#include <unistd.h> 
EMU::EMU(int argv,char **argc) : cart(argv<2?nullptr:argc[1])
{
    if(argv<2){
        cout<<"NO ARGUMENTS"<<'\n';
        return;
    }
    paused = false;
    running = true;
    while (running)
    {
        if(paused){
            sleep(1);
        }
        else{
            
        }
    }
    

}

EMU::~EMU()
{
}