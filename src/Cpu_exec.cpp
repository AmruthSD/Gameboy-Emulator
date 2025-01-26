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

    default:
        break;
    } 
}

//     [IN_AND] = proc_and,
//     [IN_XOR] = proc_xor,
//     [IN_OR] = proc_or,
//     [IN_CP] = proc_cp,
//     [IN_CB] = proc_cb,
//     [IN_RRCA] = proc_rrca,
//     [IN_RLCA] = proc_rlca,
//     [IN_RRA] = proc_rra,
//     [IN_RLA] = proc_rla,
//     [IN_STOP] = proc_stop,
//     [IN_HALT] = proc_halt,
//     [IN_DAA] = proc_daa,
//     [IN_CPL] = proc_cpl,
//     [IN_SCF] = proc_scf,
//     [IN_CCF] = proc_ccf,
//     [IN_EI] = proc_ei,
//     [IN_RETI] = proc_reti
//     [IN_DI] = proc_di,