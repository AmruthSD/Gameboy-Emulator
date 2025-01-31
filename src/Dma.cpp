#include "Dma.h"

void Dma::dma_start(uint8_t start) {
    active = true;
    byte = 0;
    start_delay = 2;
    value = start;
}

void Dma::dma_tick() {
    if (!active) {
        return;
    }
    if (start_delay) {
        start_delay--;
        return;
    }
    ppu->ppu_oam_write(byte, bus->bus_read((value * 0x100) + byte));
    byte++;
    active = byte < 0xA0;
}

bool Dma::dma_transferring() {
    return active;
}