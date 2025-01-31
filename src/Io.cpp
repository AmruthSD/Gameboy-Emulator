#include "Io.h"

uint8_t Io::io_read(uint16_t address){
    if (address == 0xFF00) {
        return gamepad->gamepad_get_output();
    }

    if (address == 0xFF01) {
        return serial_data[0];
    }

    if (address == 0xFF02) {
        return serial_data[1];
    }

    if (BETWEEN(address, 0xFF04, 0xFF07)) {
        return timer->timer_read(address);
    }

    if (address == 0xFF0F) {
        return cpu->cpu_get_int_flags();
    }

    if (BETWEEN(address, 0xFF10, 0xFF3F)) {
        //ignore sound
        return 0;
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B)) {
        return lcd->lcd_read(address);
    }
    return 0;
}

void Io::io_write(uint16_t address, uint8_t value){
    if (address == 0xFF00) {
        gamepad->gamepad_set_sel(value);
        return;
    }
    
    if (address == 0xFF01) {
        serial_data[0] = value;
        return;
    }

    if (address == 0xFF02) {
        serial_data[1] = value;
        return;
    }

    if (BETWEEN(address, 0xFF04, 0xFF07)) {
        timer->timer_write(address, value);
        return;
    }
    
    if (address == 0xFF0F) {
        cpu->cpu_set_int_flags(value);
        return;
    }

    if (BETWEEN(address, 0xFF10, 0xFF3F)) {
        //ignore sound
        return;
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B)) {
        lcd->lcd_write(address, value);
        return;
    }
}
