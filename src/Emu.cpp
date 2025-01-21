#include "Emu.h"

EMU::EMU(int argv,char **argc) : cart(argv<2?nullptr:argc[1])
{
    if(argv<2){
        cout<<"NO ARGUMENTS"<<'\n';
        return;
    }
}

EMU::~EMU()
{
}