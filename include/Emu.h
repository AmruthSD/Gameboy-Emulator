#ifndef EMU_H
#define EMU_H

#include <iostream>
#include "Cart.h"
using namespace std;

class EMU
{
private:
    Cartridge cart;
public:
    
    EMU(int argv,char **argc);
    ~EMU();
};

#endif