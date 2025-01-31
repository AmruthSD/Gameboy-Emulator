#include "Gamepad.h"

void Gamepad::gamepad_init(){

}

bool Gamepad::gamepad_button_sel(){
    return button_sel;
}

bool Gamepad::gamepad_dir_sel(){
    return dir_sel;
}

void Gamepad::gamepad_set_sel(uint8_t value){
    button_sel = value & 0x20;
    dir_sel = value & 0x02;
}

gamepad_state *Gamepad::gamepad_get_state(){
    return &controller;
}

uint8_t Gamepad::gamepad_get_output(){
    uint8_t output = 0xCF;

    if (!gamepad_button_sel()) {
        if (gamepad_get_state()->start) {
            output &= ~(1 << 3);
        } 
        if (gamepad_get_state()->select) {
            output &= ~(1 << 2);
        } 
        if (gamepad_get_state()->a) {
            output &= ~(1 << 0);
        } 
        if (gamepad_get_state()->b) {
            output &= ~(1 << 1);
        }
    }

    if (!gamepad_dir_sel()) {
        if (gamepad_get_state()->left) {
            output &= ~(1 << 1);
        } 
        if (gamepad_get_state()->right) {
            output &= ~(1 << 0);
        } 
        if (gamepad_get_state()->up) {
            output &= ~(1 << 2);
        } 
        if (gamepad_get_state()->down) {
            output &= ~(1 << 3);
        }
    }

    return output;
}
