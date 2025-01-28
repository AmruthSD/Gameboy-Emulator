#include "Cpu.h"
#include <iostream>
using namespace  std;

void Cpu::cpu_fetch_data(){
    mem_dest = 0;
    dest_is_mem = false;
    
    switch (inst.mode)
    {
    case AM_IMP:
        return;
    
    case AM_R:
        fetched_data = cpu_read_reg(inst.reg_1);
        return;

    case AM_R_R:
        fetched_data = cpu_read_reg(inst.reg_2);
        return;

    case AM_R_D8:
        fetched_data = bus->bus_read(regs.pc);
        regs.pc++;
        cpu_cycles(1);
        return;

    case AM_R_D16:
        cpu_cycles(1);
        fetched_data = bus->bus_read(regs.pc);
        fetched_data |= ((bus->bus_read(regs.pc + 1)) << 8);
        cpu_cycles(1);
        regs.pc += 2;
        return;
    
    case AM_D16:
        fetched_data = bus->bus_read(regs.pc);
        cpu_cycles(1);
        fetched_data |= ((bus->bus_read(regs.pc + 1)) << 8);
        cpu_cycles(1);
        regs.pc += 2;
        return;

    case AM_MR_R:
        dest_is_mem = true;
        mem_dest = cpu_read_reg(inst.reg_1);
        fetched_data = cpu_read_reg(inst.reg_2);
        if(inst.reg_1==RT_C){
            mem_dest |= 0xFF00;
        }
        return;

    case AM_R_MR:
        uint16_t addr = cpu_read_reg(inst.reg_2);
        if(inst.reg_1==RT_C){
            addr |= 0xFF00;
        }
        fetched_data = bus->bus_read(addr);
        cpu_cycles(1);
        return;

    case AM_R_HLI:
        fetched_data = bus->bus_read(cpu_read_reg(inst.reg_2));
        cpu_cycles(1);
        cpu_set_reg(RT_HL,cpu_read_reg(RT_HL)+1);
        return;
    
    case AM_R_HLD:
        fetched_data = bus->bus_read(cpu_read_reg(inst.reg_2));
        cpu_cycles(1);
        cpu_set_reg(RT_HL,cpu_read_reg(RT_HL)-1);
        return;

    case AM_HLI_R:
        fetched_data = cpu_read_reg(inst.reg_2);
        dest_is_mem = true;
        mem_dest = cpu_read_reg(inst.reg_1);
        cpu_set_reg(RT_HL,cpu_read_reg(RT_HL)+1);
        return;

    case AM_HLD_R:
        fetched_data = cpu_read_reg(inst.reg_2);
        dest_is_mem = true;
        mem_dest = cpu_read_reg(inst.reg_1);
        cpu_set_reg(RT_HL,cpu_read_reg(RT_HL)-1);
        return;
    
    case AM_R_A8:
        fetched_data = bus->bus_read(regs.pc);
        cpu_cycles(1);
        regs.pc++;
        return;

    case AM_A8_R:
        dest_is_mem = true;
        mem_dest = bus->bus_read(regs.pc) | 0xFF00;
        cpu_cycles(1);
        regs.pc++;
        return;

    case AM_HL_SPR:
        fetched_data = bus->bus_read(regs.pc);
        cpu_cycles(1);
        regs.pc++;
        return;

    case AM_D8:
        fetched_data = bus->bus_read(regs.pc);
        cpu_cycles(1);
        regs.pc++;
        return;

    case AM_A16_R:
    case AM_D16_R:
        dest_is_mem = true;
        uint16_t lo = bus->bus_read(regs.pc);
        cpu_cycles(1);
        uint16_t hi = bus->bus_read(regs.pc+1);
        cpu_cycles(1);
        mem_dest = (hi << 8) | lo;
        regs.pc += 2;
        fetched_data = cpu_read_reg(inst.reg_2);
        return;
    
    case AM_MR_D8:
        fetched_data = bus->bus_read(regs.pc);
        cpu_cycles(1);
        regs.pc++;
        dest_is_mem = true;
        mem_dest = cpu_read_reg(inst.reg_1);
        return;
    
    case AM_MR:
        dest_is_mem = true;
        mem_dest = cpu_read_reg(inst.reg_1);
        fetched_data = bus->bus_read(mem_dest);
        cpu_cycles(1);
        return;
    
    case AM_R_A16:
        uint16_t lo = bus->bus_read(regs.pc);
        cpu_cycles(1);
        uint16_t hi = bus->bus_read(regs.pc +1);
        cpu_cycles(1);
        uint16_t addr = lo | (hi<<8);
        regs.pc+=2;
        fetched_data = bus->bus_read(addr);
        return;



    default:
        cout<<"Invalid Addressing Mode: "<<hex<<inst.mode<< " Opcode: "<<hex<<cur_opcode<<endl;
        break;
    }
}
