#ifndef CPU_H
#define CPU_H

#include<iostream>
#include "Instruction.h"
#include "Bus.h"
#include "Timer.h"
#include <stack>
#include "Common.h"


#define CPU_FLAG_Z BIT(regs.f, 7)
#define CPU_FLAG_N BIT(regs.f, 6)
#define CPU_FLAG_H BIT(regs.f, 5)
#define CPU_FLAG_C BIT(regs.f, 4)

using namespace std;

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

typedef enum {
    IT_VBLANK = 1,
    IT_LCD_STAT = 2,
    IT_TIMER = 4,
    IT_SERIAL = 8,
    IT_JOYPAD = 16
} interrupt_type;

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
    Timer *timer;
    
    uint64_t ticks;

    bool halted;
    bool stepping;
    bool int_master_enabled;
    bool enabling_ime;
    uint8_t ie_register;
    uint8_t int_flags;

public:
    
    
    Cpu(/* args */);
    bool cpu_step();
    void cpu_execute();
    void cpu_fetch_data();
    void cpu_cycles(int num);

    uint16_t cpu_read_reg(reg_type rt);
    uint8_t cpu_read_reg8(reg_type rt);
    void cpu_set_reg(reg_type rt, uint16_t val);
    void cpu_set_reg8(reg_type rt, uint8_t val);
    
    bool is_16_bit_reg(reg_type rt);

    void stack_push(uint8_t data);
    void stack_push16(uint16_t data);
    uint8_t stack_pop();
    uint16_t stack_pop16();

    void cpu_exec_ld();
    void cpu_exec_ldh();
    
    void cpu_exec_jp();
    void cpu_exec_pop();
    void cpu_exec_push();
    void cpu_exec_jr();
    void cpu_exec_call();
    void cpu_exec_rst();
    void cpu_exec_ret();
    
    void cpu_set_flags(int8_t z, int8_t n, int8_t h, int8_t c);
    void cpu_exec_inc();
    void cpu_exec_dec();
    void cpu_exec_add();
    void cpu_exec_sub();
    void cpu_exec_sbc();
    void cpu_exec_adc();

    void cpu_exec_and();
    void cpu_exec_or();
    void cpu_exec_xor();
    void cpu_exec_cp();

    void cpu_exec_cb();

    void cpu_exec_rlca();
    void cpu_exec_rrca();
    void cpu_exec_rla();
    void cpu_exec_rra();

    void cpu_exec_daa();
    void cpu_exec_cpl();
    void cpu_exec_scf();
    void cpu_exec_ccf();

    void cpu_exec_stop();
    void cpu_exec_ei();
    void cpu_exec_di();
    void cpu_exec_halt();
    void cpu_exec_reti();
    
    void cpu_request_interrupt(interrupt_type t);
    void cpu_handle_interrupts();
    void int_handle( uint16_t address);
    bool int_check(uint16_t address, interrupt_type it);
    uint8_t cpu_get_ie_register();
    void cpu_set_ie_register(uint8_t value);
};

#endif