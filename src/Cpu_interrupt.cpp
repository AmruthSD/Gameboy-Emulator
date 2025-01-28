#include "Cpu.h"

void Cpu::int_handle( uint16_t address) {
    stack_push16(regs.pc);
    regs.pc = address;
}

bool Cpu::int_check(uint16_t address, interrupt_type it) {
    if (int_flags & it && ie_register & it) {
        int_handle(address);
        int_flags &= ~it;
        halted = false;
        int_master_enabled = false;

        return true;
    }

    return false;
}

void Cpu::cpu_handle_interrupts() {
    if (int_check(0x40, IT_VBLANK)) {

    } else if (int_check(0x48, IT_LCD_STAT)) {

    } else if (int_check(0x50, IT_TIMER)) {

    }  else if (int_check(0x58, IT_SERIAL)) {

    }  else if (int_check(0x60, IT_JOYPAD)) {

    } 
}

void Cpu::cpu_request_interrupt(interrupt_type t){
    int_flags |= t;
}

uint8_t Cpu::cpu_get_ie_register(){
    return ie_register;
}

void Cpu::cpu_set_ie_register(uint8_t value){
    ie_register = value;
}