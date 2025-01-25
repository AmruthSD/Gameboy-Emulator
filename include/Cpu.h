#ifndef CPU_H
#define CPU_H

#include<iostream>
#include "Instruction.h"
#include "Bus.h"

typedef struct {
    uint8_t a;
    uint8_t f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t pc;
    uint16_t sp;
} cpu_registers;

class Cpu
{
private:

    cpu_registers regs;
    uint16_t fetched_data;
    uint16_t mem_dest;
    bool dest_is_mem;
    uint8_t cur_opcode;
    instruction inst;
    Bus *bus;

public:

    Cpu(/* args */);
    bool cpu_step();
    void cpu_execute();
    void cpu_fetch_data();
    uint16_t cpu_read_reg(reg_type rt);
    uint8_t cpu_read_reg8(reg_type rt);
    void cpu_set_reg(reg_type rt, uint16_t val);
    void cpu_set_reg8(reg_type rt, uint8_t val);
    
};

#endif