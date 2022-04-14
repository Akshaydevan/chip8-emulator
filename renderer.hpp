#pragma once

#include <array>
#include <SFML/Graphics.hpp>

class Renderer {
public:
    Renderer() = default;

    void init();
    bool poll();
    void render(std::array<bool, 2048>&);

private:
    sf::RenderWindow m_window;
    std::vector<sf::RectangleShape> m_list;
};
