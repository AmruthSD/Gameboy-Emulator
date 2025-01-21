#ifndef ROM_H
#define ROM_H

#include <iostream>
#include <vector>
using namespace std;

class ROM
{
    private:
    public:
        vector<uint8_t> rom = vector<uint8_t>(256);
        ROM(char *filename);
        ~ROM();
};


#endif