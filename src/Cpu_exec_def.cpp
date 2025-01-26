#include "Cpu.h"

bool Cpu::is_16_bit_reg(reg_type rt){
    return rt >= RT_AF;
}


void Cpu::cpu_exec_ld(){
    if(dest_is_mem){
        if(is_16_bit_reg(inst.reg_2)){
            bus->bus_write16(mem_dest,fetched_data);
        }
        else{
            bus->bus_write(mem_dest,fetched_data);
        }
    }
    else{
        if(inst.mode == AM_HL_SPR){
            // spr
        }
        else{
            cpu_set_reg(inst.reg_1,fetched_data);
        }
    }
}

void Cpu::cpu_exec_ldh(){
    if(inst.reg_1 == RT_A){
        cpu_set_reg(inst.reg_1,bus->bus_read(0xFF00|fetched_data));
    }
    else{
        bus->bus_write(mem_dest,regs.a);
    }
}

bool check_cond(bool z,bool c,instruction inst){
    switch (inst.cond)
    {
        case CT_NONE: return true;
        case CT_C: return c;
        case CT_NC: return !c;
        case CT_Z: return z;
        case CT_NZ: return !z;
    }

    return false;
}

void Cpu::cpu_exec_jp(){
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;
    bool pos = check_cond(z,c,inst);
    if(pos){
        regs.pc = fetched_data;
    }
}

void Cpu::cpu_exec_jr(){
    int8_t rel = (int8_t)(fetched_data & 0xFF);
    uint16_t addr = regs.pc + rel;
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;
    bool pos = check_cond(z,c,inst);
    if(pos){
        regs.pc = addr;
    }
}

void Cpu::cpu_exec_call(){
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;
    bool pos = check_cond(z,c,inst);
    if(pos){
        stack_push16(regs.pc);
        regs.pc = fetched_data;
    }
}

void Cpu::cpu_exec_rst(){
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;
    bool pos = check_cond(z,c,inst);
    if(pos){
        stack_push16(regs.pc);
        regs.pc = inst.param;
    }
}

void Cpu::cpu_exec_ret(){
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;
    bool pos = check_cond(z,c,inst);
    if(pos){
        uint16_t lo = stack_pop();
        uint16_t hi = stack_pop();
        uint16_t n = (hi<<8) | lo;
        regs.pc = n;
    }
}

void Cpu::cpu_exec_pop(){
    uint16_t lo = stack_pop();
    uint16_t hi = stack_pop();
    uint16_t n = (hi << 8)| lo;
    cpu_set_reg(inst.reg_1,n);
    if(inst.reg_1==RT_AF){
        cpu_set_reg(inst.reg_1,n & 0xFFF0);
    }
}

void Cpu::cpu_exec_push(){
    uint16_t hi = (cpu_read_reg(inst.reg_1)>>8) & 0xFF;
    stack_push(hi);
    uint16_t lo = cpu_read_reg(inst.reg_1) & 0xFF;
    stack_push(lo);
}

