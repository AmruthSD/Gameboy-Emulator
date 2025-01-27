#include "Cpu.h"
#include "Instruction.h"

Cpu::Cpu(/* args */)
{
    regs.pc = 0x100;
    regs.a = 0x01;
}

bool Cpu::cpu_step(){

    if(!halted){
        cur_opcode = bus->bus_read(regs.pc);
        regs.pc++;
        inst = instruction_from_opcode(cur_opcode);
        
        cpu_execute();
    }
    else{
        if(int_flags){
            halted = false;
        }
    }

    if(int_master_enabled){
        cpu_handle_interrupts();
        enabling_ime = false;
    }

    if(enabling_ime){
        int_master_enabled = true;
    }

    return true;
}