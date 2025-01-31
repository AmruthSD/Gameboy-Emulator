#pragma once

#include <iostream>

typedef struct {
    bool start;
    bool select;
    bool a;
    bool b;
    bool up;
    bool down;
    bool left;
    bool right;
} gamepad_state;

class Gamepad
{
private:
    bool button_sel;
    bool dir_sel;
    gamepad_state controller;

public:
    void gamepad_init();
    bool gamepad_button_sel();
    bool gamepad_dir_sel();
    void gamepad_set_sel(uint8_t value);
    gamepad_state *gamepad_get_state();
    uint8_t gamepad_get_output();
};
