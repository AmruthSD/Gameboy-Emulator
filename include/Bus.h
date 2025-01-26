#pragma once

#include <iostream>

class Bus
{
private:
    /* data */
public:
    uint8_t bus_read(uint16_t address);
    void bus_write(uint16_t address, uint8_t value);
    void bus_write16(uint16_t address, uint16_t value);
};

