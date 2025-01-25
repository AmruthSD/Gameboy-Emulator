#include "Cpu.h"

uint16_t reverse(uint16_t n) {
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

uint8_t Cpu::cpu_read_reg8(reg_type rt) {
    switch(rt) {
        case RT_A: return regs.a;
        case RT_F: return regs.f;
        case RT_B: return regs.b;
        case RT_C: return regs.c;
        case RT_D: return regs.d;
        case RT_E: return regs.e;
        case RT_H: return regs.h;
        case RT_L: return regs.l;
        case RT_HL: {
            return bus->bus_read(cpu_read_reg(RT_HL));
        }
        default:
            printf("**ERR INVALID REG8: %d\n", rt);
    }
}


uint16_t Cpu::cpu_read_reg(reg_type rt){
    switch (rt) 
    {
        case RT_A: return regs.a;
        case RT_F: return regs.f;
        case RT_B: return regs.b;
        case RT_C: return regs.c;
        case RT_D: return regs.d;
        case RT_E: return regs.e;
        case RT_H: return regs.h;
        case RT_L: return regs.l;
        case RT_AF: return reverse(*((uint16_t *)&regs.a));
        case RT_BC: return reverse(*((uint16_t *)&regs.b));
        case RT_DE: return reverse(*((uint16_t *)&regs.d));
        case RT_HL: return reverse(*((uint16_t *)&regs.h));

        case RT_PC: return regs.pc;
        case RT_SP: return regs.sp;

    default:
        break;
    }
    return 0;
}

void Cpu::cpu_set_reg(reg_type rt, uint16_t val) {
    switch(rt) {
        case RT_A: regs.a = val & 0xFF; break;
        case RT_F: regs.f = val & 0xFF; break;
        case RT_B: regs.b = val & 0xFF; break;
        case RT_C: regs.c = val & 0xFF; break;
        case RT_D: regs.d = val & 0xFF; break;
        case RT_E: regs.e = val & 0xFF; break;
        case RT_H: regs.h = val & 0xFF; break;
        case RT_L: regs.l = val & 0xFF; break;

        case RT_AF: *((uint16_t *)&regs.a) = reverse(val); break;
        case RT_BC: *((uint16_t *)&regs.b) = reverse(val); break;
        case RT_DE: *((uint16_t *)&regs.d) = reverse(val); break;
        case RT_HL: *((uint16_t *)&regs.h) = reverse(val); break;
        

        case RT_PC: regs.pc = val; break;
        case RT_SP: regs.sp = val; break;
        case RT_NONE: break;
    }
}


void Cpu::cpu_set_reg8(reg_type rt, uint8_t val) {
    switch(rt) {
        case RT_A: regs.a = val & 0xFF; break;
        case RT_F: regs.f = val & 0xFF; break;
        case RT_B: regs.b = val & 0xFF; break;
        case RT_C: regs.c = val & 0xFF; break;
        case RT_D: regs.d = val & 0xFF; break;
        case RT_E: regs.e = val & 0xFF; break;
        case RT_H: regs.h = val & 0xFF; break;
        case RT_L: regs.l = val & 0xFF; break;
        case RT_HL: bus->bus_write(cpu_read_reg(RT_HL), val); break;
        default:
            printf("**ERR INVALID REG8: %d\n", rt);
    }
}