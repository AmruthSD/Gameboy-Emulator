#include "Cpu.h"
#include "Instruction.h"

Cpu::Cpu(/* args */)
{
    regs.pc = 0x100;
    regs.sp = 0xFFFE;
    *((short *)&regs.a) = 0xB001;
    *((short *)&regs.b) = 0x1300;
    *((short *)&regs.d) = 0xD800;
    *((short *)&regs.h) = 0x4D01;
    ie_register = 0;
    int_flags = 0;
    int_master_enabled = false;
}

bool Cpu::cpu_step(){

    if(!halted){
        cur_opcode = bus->bus_read(regs.pc);
        regs.pc++;
        inst = instruction_from_opcode(cur_opcode);
        cpu_cycles(1);
        cpu_execute();
    }
    else{
        cpu_cycles(1);
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

void Cpu::cpu_cycles(int num){
    int n = num * 4;
    for (int i = 0; i < n; i++)
    {
        ticks++;
        timer->timer_tick();
    }
    
}