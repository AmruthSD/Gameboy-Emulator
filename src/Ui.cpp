#include "Ui.h"
#include <unistd.h>
using namespace std;
int scale = 4;

void Ui::ui_init(){
    
    Window.create(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Gameboy");
    sfmlTexture = sf::RenderTexture({SCREEN_WIDTH, SCREEN_HEIGHT});
}

void Ui::delay(uint32_t ms) {
    sf::sleep(sf::milliseconds(ms));
}

uint32_t Ui::get_ticks() {
    return static_cast<uint32_t>(sf::Clock().getElapsedTime().asMilliseconds());
}

static sf::Color tile_colors[4] = {sf::Color(255, 255, 255), sf::Color(170, 170, 170), sf::Color(85, 85, 85), sf::Color(0, 0, 0)};


void Ui::ui_update() {
    sf::RectangleShape rect(sf::Vector2f(scale, scale));
    
    uint32_t *video_buffer = ppu->get_video_buffer();

    for (int line_num = 0; line_num < YRES; line_num++) {
        for (int x = 0; x < XRES; x++) {
            rect.setPosition(sf::Vector2f(x * scale, line_num * scale));
            rect.setFillColor(sf::Color(video_buffer[x + (line_num * XRES)]));
            sfmlTexture.draw(rect);
        }
    }

    sfmlTexture.display();
    Window.clear();
    Window.draw(sf::Sprite(sfmlTexture.getTexture()));
    Window.display();

    //update_dbg_window();
}


void Ui::ui_on_key_press(bool down,const struct sf::Event::KeyPressed *key_code) {
    switch(key_code->scancode) {
        case sf::Keyboard::Scancode::Z: gamepad->gamepad_get_state()->b = down; break;
        case sf::Keyboard::Scancode::X: gamepad->gamepad_get_state()->a = down; break;
        case sf::Keyboard::Scancode::Space: gamepad->gamepad_get_state()->start = down; break;
        case sf::Keyboard::Scancode::Tab: gamepad->gamepad_get_state()->select = down; break;
        case sf::Keyboard::Scancode::Up: gamepad->gamepad_get_state()->up = down; break;
        case sf::Keyboard::Scancode::Down: gamepad->gamepad_get_state()->down = down; break;
        case sf::Keyboard::Scancode::Left: gamepad->gamepad_get_state()->left = down; break;
        case sf::Keyboard::Scancode::Right: gamepad->gamepad_get_state()->right = down; break;
    }
}

void Ui::ui_on_key_release(bool down,const struct sf::Event::KeyReleased *key_code) {
    switch(key_code->scancode) {
        case sf::Keyboard::Scancode::Z: gamepad->gamepad_get_state()->b = down; break;
        case sf::Keyboard::Scancode::X: gamepad->gamepad_get_state()->a = down; break;
        case sf::Keyboard::Scancode::Space: gamepad->gamepad_get_state()->start = down; break;
        case sf::Keyboard::Scancode::Tab: gamepad->gamepad_get_state()->select = down; break;
        case sf::Keyboard::Scancode::Up: gamepad->gamepad_get_state()->up = down; break;
        case sf::Keyboard::Scancode::Down: gamepad->gamepad_get_state()->down = down; break;
        case sf::Keyboard::Scancode::Left: gamepad->gamepad_get_state()->left = down; break;
        case sf::Keyboard::Scancode::Right: gamepad->gamepad_get_state()->right = down; break;
    }
}

void Ui::ui_handle_events() {
    while(Window.isOpen()){
        ui_update();
        while (const std::optional event = Window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>()){
                Window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                ui_on_key_press(true,keyPressed);
            }
            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                ui_on_key_release(false,keyReleased);
            }
        }
    }
}
