#pragma once

#include "Common.h"
#include "Bus.h"
#include "Ppu.h"
#include "Gamepad.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

class Bus;
class Ppu;
class Gamepad;

class Ui
{
public:
    sf::RenderWindow Window;
    sf::RenderTexture sfmlTexture;

    Bus *bus;
    Ppu *ppu;
    Gamepad *gamepad;
public:
    void ui_init();
    void ui_handle_events();
    void ui_update();
    void delay(uint32_t ms);
    uint32_t get_ticks();

    void display_tile(sf::Image& surface, uint16_t startLocation, uint16_t tileNum, int x, int y);
    void update_dbg_window();
    void ui_on_key_press(bool down,const struct sf::Event::KeyPressed *key_code);
    void ui_on_key_release(bool down,const struct sf::Event::KeyReleased *key_code);
};