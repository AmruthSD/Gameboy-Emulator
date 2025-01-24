#include "Cpu.h"
#include "Instruction.h"

Cpu::Cpu(/* args */)
{
    regs.pc = 0x100;
    regs.a = 0x01;
}

bool Cpu::cpu_step(){
    cur_opcode = bus->bus_read(regs.pc);
    regs.pc++;
    inst = instruction_from_opcode(cur_opcode);
    
    cpu_execute();
}