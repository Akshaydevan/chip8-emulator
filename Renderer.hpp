#pragma once

#include <array>
#include <SFML/Graphics.hpp>

class Renderer {
public:
    Renderer() = default;

    void init();
    void poll();
    void render(std::array<bool, 2048>&);

private:
    sf::RenderWindow m_window;
};
