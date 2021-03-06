#include "sfml_screen.h"

#include "../../util/log.h"

SFMLScreen::SFMLScreen(std::shared_ptr<Input> inInput, bool _whole_framebuffer, uint _magnification) :
    input(inInput),
    magnification(_magnification),
    whole_framebuffer(_whole_framebuffer),
    logical_width(GAMEBOY_WIDTH),
    logical_height(GAMEBOY_HEIGHT),
    width(logical_width*magnification),
    height(logical_height*magnification),
    pixel_size(magnification),
    window(sf::VideoMode(width, height), "Emulator", sf::Style::Titlebar | sf::Style::Close)
{
    image.create(width, height);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
}

void SFMLScreen::draw(const FrameBuffer& buffer) {
    window.clear(sf::Color::White);

    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            input->on_key_pressed(event.key.code);
        }

        if (event.type == sf::Event::KeyReleased) {
            input->on_key_released(event.key.code);
        }

        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    set_pixels(buffer);
    texture.loadFromImage(image);
    sprite.setTexture(texture, true);

    window.draw(sprite);

    window.display();
}

void SFMLScreen::set_pixels(const FrameBuffer& buffer) {
    for (uint y = 0; y < GAMEBOY_HEIGHT; y++) {
        for (uint x = 0; x < GAMEBOY_WIDTH; x++) {
            Color color = buffer.get_pixel(x, y);
            sf::Color pixel_color = get_real_color(color);

            set_large_pixel(x, y, pixel_color);
        }
    }
}

void SFMLScreen::set_large_pixel(uint x, uint y, sf::Color color) {
    for (uint w = 0; w < pixel_size; w++) {
        for (uint h = 0; h < pixel_size; h++) {
            image.setPixel(x * pixel_size + w, y * pixel_size + h, color);
        }
    }
}

sf::Color SFMLScreen::get_real_color(Color color) {
    switch (color) {
        case Color::White: return sf::Color(255, 255, 255);
        case Color::LightGray: return sf::Color(170, 170, 170);
        case Color::DarkGray: return sf::Color( 85,  85,  85);
        case Color::Black: return sf::Color(  0,   0,   0);
    }
}

bool SFMLScreen::is_open() {
    return window.isOpen();
}
