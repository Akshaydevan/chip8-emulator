#include <SFML/Graphics.hpp>
#include "renderer.hpp"


void Renderer::init(){
    m_window.create(sf::VideoMode(640,320), "chip8");
    m_window.clear(sf::Color::Black);
}

void Renderer::poll(){
    sf::Event evt;

    while(m_window.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            m_window.close();
        }
    }
}

void Renderer::render(std::array<bool, 2048>& displayBuffer) {
    std::vector<sf::RectangleShape> boxes;
    int xpos = 0;
    int ypos = 0;

    for (auto &i : displayBuffer) {
        if (i == true) {
            boxes.emplace_back(sf::Vector2f(xpos, ypos));
        }

        xpos += 10;

        if (xpos >= 640) {
            xpos = 0;
            ypos++;
        }
    }
}
