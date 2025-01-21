#include <iostream>
#include <vector>
#include "Rom.h"
using namespace std;



int main(int argv,char ** argc){
    if(argv<2){
        cout<<"NO ARGUMENTS"<<'\n';
        return 0;
    }
    ROM rom(argc[1]);
    for (int i = 0; i < 256; i++)
    {
        cout<<(char)rom.rom[i];
    }
    cout<<'\n';
    
    return 0;
}