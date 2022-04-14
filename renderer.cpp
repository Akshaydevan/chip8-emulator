#include <SFML/Graphics.hpp>
#include "renderer.hpp"


void Renderer::init(){
    m_window.create(sf::VideoMode(640,330), "chip8");
    m_window.clear(sf::Color::Black);

    int xpos = 0;
    int ypos = 0;

    for(int i = 0; i < 2048; i++) {
        sf::RectangleShape box;
        box.setSize(sf::Vector2f(10, 10));
        box.setPosition(sf::Vector2f(xpos, ypos));
        box.setFillColor(sf::Color::White);
        m_list.push_back(box);

        xpos += 10;
        if (xpos >= 640) {
            xpos = 0;
            ypos += 10;
        }
    }
}

bool Renderer::poll(){
    sf::Event evt;

    while(m_window.pollEvent(evt)) {
        if (evt.type == sf::Event::Closed) {
            m_window.close();
            return false;
        }
    }

    return true;
}

void Renderer::render(std::array<bool, 2048>& displayBuffer) {
    std::vector<sf::RectangleShape> boxes;
    int pos = 0;

    for (auto &i : displayBuffer) {
        if(i == true) {
            m_window.draw(m_list[pos]);
        }

        pos++;
    }

    m_window.display();
}
