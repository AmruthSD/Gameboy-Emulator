#ifndef ROM_H
#define ROM_H

#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
using namespace std;

typedef struct {

    uint8_t entry[4];
    uint8_t logo[0x30];
    char title[16];
    uint16_t new_lic_code;
    uint8_t sgb_flag;
    uint8_t type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t dest_code;
    uint8_t lic_code;
    uint8_t version;
    uint8_t checksum;
    uint16_t global_checksum;

} rom_header;




class Cartridge
{
    public:
        rom_header *header;
        vector<uint8_t> rom_data;
        uint32_t rom_size;
        string filename;
    
        
    public:
        Cartridge(char *rom_name);
        const char *cart_lic_name();
        const char *cart_type_name();
        
};


#endif