#include "Cpu.h"
#include "Instruction.h"

void Cpu::cpu_execute(){
    
    switch (inst.type)
    {
    case IN_NONE:
        return;
    
    case IN_NOP:
        return;
    
    case IN_LD:
        cpu_exec_ld();
        return;

    case IN_LDH:
        cpu_exec_ldh();
        return;

    case IN_JP:
        cpu_exec_jp();
        return;

    case IN_JR:
        cpu_exec_jr();
        return;

    case IN_POP:
        cpu_exec_pop();
        return;
    
    case IN_PUSH:
        cpu_exec_push();
        return;

    case IN_CALL:
        cpu_exec_call();
        return;

    case IN_RST:
        cpu_exec_rst();
        return;

    case IN_RET:
        cpu_exec_ret();
        return;

    case IN_INC:
        cpu_exec_inc();
        return;
    
    case IN_DEC:
        cpu_exec_dec();
        return;

    case IN_SUB:
        cpu_exec_sub();
        return;
    
    case IN_SBC:
        cpu_exec_sbc();
        return;
    
    case IN_ADD:
        cpu_exec_add();
        return;
    
    case IN_ADC:
        cpu_exec_adc();
        return;

    case IN_OR:
        cpu_exec_or();
        return;
    
    case IN_AND:
        cpu_exec_and();
        return;

    case IN_XOR:
        cpu_exec_xor();
        return;
    
    case IN_CP:
        cpu_exec_cp();
        return;

    case IN_CB:
        cpu_exec_cb();
        return;

    case IN_RRCA:
        cpu_exec_rrca();
        return;
    
    case IN_RLCA:
        cpu_exec_rlca();
        return;
    
    case IN_RRA:
        cpu_exec_rra();
        return;

    case IN_RLA:
        cpu_exec_rla();
        return;

    case IN_DAA:
        cpu_exec_daa();
        return;

    case IN_CPL:
        cpu_exec_cpl();
        return;
    
    case IN_SCF:
        cpu_exec_scf();
        return;

    case IN_CCF:
        cpu_exec_ccf();
        return;
    
    case IN_HALT:
        cpu_exec_halt();
        return;

    case IN_EI:
        cpu_exec_ei();
        return;

    case IN_RETI:
        cpu_exec_reti();
        return;

    case IN_DI:
        cpu_exec_di();
        return;
    
    case IN_STOP:
        cpu_exec_stop();
        return;

    default:
        break;
    } 
}
