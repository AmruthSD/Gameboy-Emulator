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


// Arithmetic 

void Cpu::cpu_set_flags(int8_t z, int8_t n, int8_t h, int8_t c) {
    if (z != -1) {
        BIT_SET(regs.f, 7, z);
    }

    if (n != -1) {
        BIT_SET(regs.f, 6, n);
    }

    if (h != -1) {
        BIT_SET(regs.f, 5, h);
    }

    if (c != -1) {
        BIT_SET(regs.f, 4, c);
    }
}

void Cpu::cpu_exec_inc(){
    uint16_t  val = cpu_read_reg(inst.reg_1) + 1;

    if(inst.reg_1 == RT_HL && inst.mode == AM_MR){
        val = bus->bus_read(cpu_read_reg(RT_HL)) + 1;
        val &= 0xFF;
        bus->bus_write(cpu_read_reg(RT_HL),val);
    }
    else{
        cpu_set_reg(inst.reg_1,val);
        val = cpu_read_reg(inst.reg_1);
    }

    if((cur_opcode & 0x03) == 0x03){
        return;
    }

    cpu_set_flags(val == 0, 0, (val & 0x0F) == 0, -1);

}

void Cpu::cpu_exec_dec(){
    uint16_t  val = cpu_read_reg(inst.reg_1) - 1;

    if(inst.reg_1 == RT_HL && inst.mode == AM_MR){
        val = bus->bus_read(cpu_read_reg(RT_HL)) - 1;
        bus->bus_write(cpu_read_reg(RT_HL),val);
    }
    else{
        cpu_set_reg(inst.reg_1,val);
        val = cpu_read_reg(inst.reg_1);
    }

    if((cur_opcode & 0x0B) == 0x0B){
        return;
    }

    cpu_set_flags(val == 0, 1, (val & 0x0F) == 0x0F, -1);
}


void Cpu::cpu_exec_sub(){
    uint16_t val = cpu_read_reg(inst.reg_1) - fetched_data;

    int z = val == 0;
    int h = ((int)cpu_read_reg(inst.reg_1) & 0xF) - ((int)fetched_data & 0xF) < 0;
    int c = ((int)cpu_read_reg(inst.reg_1)) - ((int)fetched_data) < 0;

    cpu_set_reg(inst.reg_1, val);
    cpu_set_flags(z, 1, h, c);
}

void Cpu::cpu_exec_sbc(){
    uint8_t val = fetched_data + CPU_FLAG_C;

    int z = cpu_read_reg(inst.reg_1) - val == 0;

    int h = ((int)cpu_read_reg(inst.reg_1) & 0xF) - ((int)fetched_data & 0xF) - ((int)CPU_FLAG_C) < 0;
    int c = ((int)cpu_read_reg(inst.reg_1)) - ((int)fetched_data) - ((int)CPU_FLAG_C) < 0;

    cpu_set_reg(inst.reg_1, cpu_read_reg(inst.reg_1) - val);
    cpu_set_flags(z, 1, h, c);
}

void Cpu::cpu_exec_adc(){
    uint16_t u = fetched_data;
    uint16_t a = regs.a;
    uint16_t c = CPU_FLAG_C;

    regs.a = (a + u + c) & 0xFF;

    cpu_set_flags(regs.a == 0, 0, (a & 0xF) + (u & 0xF) + c > 0xF, a + u + c > 0xFF);
}

void Cpu::cpu_exec_add(){
    uint32_t val = cpu_read_reg(inst.reg_1) + fetched_data;
    bool is_16_bit = is_16_bit_reg(inst.reg_1);

    if(inst.reg_1 == RT_SP){
        val = cpu_read_reg(inst.reg_1) + (int8_t)fetched_data;
    }
    int z = (val & 0xFF) == 0;
    int h = (cpu_read_reg(inst.reg_1) & 0xF) + (fetched_data & 0xF) >= 0x10;
    int c = (int)(cpu_read_reg(inst.reg_1) & 0xFF) + (int)(fetched_data & 0xFF) >= 0x100;
    if (is_16_bit) {
        z = -1;
        h = (cpu_read_reg(inst.reg_1) & 0xFFF) + (fetched_data & 0xFFF) >= 0x1000;
        uint32_t n = ((uint32_t)cpu_read_reg(inst.reg_1)) + ((uint32_t)fetched_data);
        c = n >= 0x10000;
    }

    if (inst.reg_1 == RT_SP) {
        z = 0;
        h = (cpu_read_reg(inst.reg_1) & 0xF) + (fetched_data & 0xF) >= 0x10;
        c = (int)(cpu_read_reg(inst.reg_1) & 0xFF) + (int)(fetched_data & 0xFF) > 0x100;
    }

    cpu_set_reg(inst.reg_1, val & 0xFFFF);
    cpu_set_flags(z, 0, h, c);
}