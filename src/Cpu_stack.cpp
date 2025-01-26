#include "Cpu.h"

void Cpu::stack_push(uint8_t data) {
    regs.sp--;
    bus->bus_write(regs.sp, data);
}

void Cpu::stack_push16(uint16_t data) {
    stack_push((data >> 8) & 0xFF);
    stack_push(data & 0xFF);
}

uint8_t Cpu::stack_pop() {
    return bus->bus_read(regs.sp++);
}

uint16_t Cpu::stack_pop16() {
    uint16_t lo = stack_pop();
    uint16_t hi = stack_pop();

    return (hi << 8) | lo;
}