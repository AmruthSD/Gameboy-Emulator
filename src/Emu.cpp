#include "Emu.h"

EMU::EMU(int argv,char **argc) : rom(argv<2?nullptr:argc[1])
{
    if(argv<2){
        cout<<"NO ARGUMENTS"<<'\n';
        return;
    }
    cout<<"Rom Contents"<<'\n';
    for (int i = 0; i < 256; i++)
    {
        cout<<(char)rom.rom[i];
    }
    cout<<'\n';
    
}

EMU::~EMU()
{
}