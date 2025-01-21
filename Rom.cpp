#include "Rom.h"


ROM::ROM(char *filename)
{
    FILE *file = fopen(filename, "rb");
    int pos = 0;
    while(fread(&rom[pos],1,1,file)){
        pos++;
    }
    fclose(file);
}

ROM::~ROM()
{
}
