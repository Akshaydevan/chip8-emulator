#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include "keyboardhandler.hpp"

class Renderer {
public:
    Renderer();

    void init();
    bool poll();
    void render(std::array<bool, 2048>&);

private:
    sf::RenderWindow m_window;
    std::vector<sf::RectangleShape> m_boxes;
    KeyboardHandler &m_keyboardHandler;
};
