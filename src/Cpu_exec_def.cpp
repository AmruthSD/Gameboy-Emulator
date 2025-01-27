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
            uint8_t hflag = (cpu_read_reg(inst.reg_2) & 0xF) + (fetched_data & 0xF) >= 0x10;
            uint8_t cflag = (cpu_read_reg(inst.reg_2) & 0xFF) + (fetched_data & 0xFF) >= 0x100;
            cpu_set_flags(0, 0, hflag, cflag);
            cpu_set_reg(inst.reg_1, cpu_read_reg(inst.reg_2) + (int8_t)fetched_data);
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


// Logical
void Cpu::cpu_exec_and(){
    regs.a &= fetched_data;
    cpu_set_flags(regs.a == 0, 0, 1, 0);
}

void Cpu::cpu_exec_or(){
    regs.a |= fetched_data & 0xFF;
    cpu_set_flags(regs.a == 0, 0, 0, 0);
}

void Cpu::cpu_exec_xor(){
    regs.a ^= fetched_data & 0xFF;
    cpu_set_flags(regs.a == 0, 0, 0, 0);
}

void Cpu::cpu_exec_cp(){
    int n = (int)regs.a - (int)fetched_data;
    cpu_set_flags(n == 0, 1, ((int)regs.a & 0x0F) - ((int)fetched_data & 0x0F) < 0, n < 0);
}

reg_type rt_lookup[] = {
    RT_B,
    RT_C,
    RT_D,
    RT_E,
    RT_H,
    RT_L,
    RT_HL,
    RT_A
};

reg_type decode_reg(uint8_t reg) {
    if (reg > 0b111) {
        return RT_NONE;
    }

    return rt_lookup[reg];
}

void Cpu::cpu_exec_cb(){
    uint8_t op = fetched_data;
    reg_type reg = decode_reg(op & 0b111);
    uint8_t bit = (op>>3) & 0b111;
    uint8_t bit_op = (op>>6) & 0b11;
    uint8_t reg_val = cpu_read_reg8(reg);

    switch(bit_op) {
        case 1:
            //BIT
            cpu_set_flags( !(reg_val & (1 << bit)), 0, 1, -1);
            return;

        case 2:
            //RST
            reg_val &= ~(1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;

        case 3:
            //SET
            reg_val |= (1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
    }

    bool flagC = CPU_FLAG_C;

    switch(bit) {
        case 0: {
            //RLC
            bool setC = false;
            uint8_t result = (reg_val << 1) & 0xFF;

            if ((reg_val & (1 << 7)) != 0) {
                result |= 1;
                setC = true;
            }

            cpu_set_reg8(reg, result);
            cpu_set_flags( result == 0, false, false, setC);
        } return;

        case 1: {
            //RRC
            uint8_t old = reg_val;
            reg_val >>= 1;
            reg_val |= (old << 7);

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, old & 1);
        } return;

        case 2: {
            //RL
            uint8_t old = reg_val;
            reg_val <<= 1;
            reg_val |= flagC;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, !!(old & 0x80));
        } return;

        case 3: {
            //RR
            uint8_t old = reg_val;
            reg_val >>= 1;

            reg_val |= (flagC << 7);

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, old & 1);
        } return;

        case 4: {
            //SLA
            uint8_t old = reg_val;
            reg_val <<= 1;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( !reg_val, false, false, !!(old & 0x80));
        } return;

        case 5: {
            //SRA
            uint8_t u = (int8_t)reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags( !u, 0, 0, reg_val & 1);
        } return;

        case 6: {
            //SWAP
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags( reg_val == 0, false, false, false);
        } return;

        case 7: {
            //SRL
            uint8_t u = reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags( !u, 0, 0, reg_val & 1);
        } return;
    }
}

//Shifting

void Cpu::cpu_exec_rlca(){
    uint8_t u = regs.a;
    bool c = (u >> 7) & 1;
    u = (u << 1) | c;
    regs.a = u;

    cpu_set_flags(0, 0, 0, c);
}

void Cpu::cpu_exec_rla(){
    uint8_t u = regs.a;
    uint8_t cf = CPU_FLAG_C;
    uint8_t c = (u >> 7) & 1;

    regs.a = (u << 1) | cf;
    cpu_set_flags(0, 0, 0, c);
}

void Cpu::cpu_exec_rrca(){
    uint8_t b = regs.a & 1;
    regs.a >>= 1;
    regs.a |= (b << 7);

    cpu_set_flags(0, 0, 0, b);
}

void Cpu::cpu_exec_rra(){
    uint8_t carry = CPU_FLAG_C;
    uint8_t new_c = regs.a & 1;

    regs.a >>= 1;
    regs.a |= (carry << 7);

    cpu_set_flags(0, 0, 0, new_c);
}

// Flags OPerations

void Cpu::cpu_exec_daa(){
    uint8_t u = 0;
    int fc = 0;
    if (CPU_FLAG_H || (!CPU_FLAG_N && (regs.a & 0xF) > 9)) {
        u = 6;
    }
    if (CPU_FLAG_C || (!CPU_FLAG_N && regs.a > 0x99)) {
        u |= 0x60;
        fc = 1;
    }
    regs.a += CPU_FLAG_N ? -u : u;

    cpu_set_flags(regs.a == 0, -1, 0, fc);
}

void Cpu::cpu_exec_cpl(){
    regs.a = ~regs.a;
    cpu_set_flags(-1, 1, 1, -1);
}

void Cpu::cpu_exec_scf(){
    cpu_set_flags( -1, 0, 0, 1);
}

void Cpu::cpu_exec_ccf(){
    cpu_set_flags( -1, 0, 0, CPU_FLAG_C ^ 1);
}

// Interrupt

void Cpu::cpu_exec_halt(){
    halted = true;
}

void Cpu::cpu_exec_stop(){
    
}

void Cpu::cpu_exec_ei(){
    enabling_ime = true;
}

void Cpu::cpu_exec_di(){
    int_master_enabled = false;
}

void Cpu::cpu_exec_reti(){
    int_master_enabled = true;
    cpu_exec_ret();
}