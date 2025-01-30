#pragma once
#include <iostream>

class Dma
{
private:
    bool active;
    uint8_t byte;
    uint8_t value;
    uint8_t start_delay;
public:
    void dma_start(uint8_t start);
    void dma_tick();
    bool dma_transferring();
};

