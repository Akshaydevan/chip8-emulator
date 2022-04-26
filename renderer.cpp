#include <SFML/Graphics.hpp>
#include "renderer.hpp"

Renderer::Renderer()
    : m_keyboardHandler(KeyboardHandler::getInstance())
{

}

void Renderer::init(){
    m_window.create(sf::VideoMode(640,330), "chip8");
    m_window.clear(sf::Color::Black);

    m_window.setFramerateLimit(500);
    int xpos = 0;
    int ypos = 0;

    for(int i = 0; i < 2048; i++) {
        sf::RectangleShape box;
        box.setSize(sf::Vector2f(10, 10));
        box.setPosition(sf::Vector2f(xpos, ypos));
        box.setFillColor(sf::Color::White);
        m_boxes.push_back(box);

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

        if (evt.type == sf::Event::KeyPressed) {
            switch(evt.key.code) {
            case sf::Keyboard::Num1:
                m_keyboardHandler.setKeyPress(1);
                break;

            case sf::Keyboard::Num2:
                m_keyboardHandler.setKeyPress(2);
                break;

            case sf::Keyboard::Num3:
                m_keyboardHandler.setKeyPress(3);
                break;

            case sf::Keyboard::Num4:
                m_keyboardHandler.setKeyPress(12);
                break;

            case sf::Keyboard::Q:
                m_keyboardHandler.setKeyPress(4);
                break;

            case sf::Keyboard::W:
                m_keyboardHandler.setKeyPress(5);
                break;

            case sf::Keyboard::E:
                m_keyboardHandler.setKeyPress(6);
                break;

            case sf::Keyboard::R:
                m_keyboardHandler.setKeyPress(13);
                break;

            case sf::Keyboard::A:
                m_keyboardHandler.setKeyPress(7);
                break;

            case sf::Keyboard::S:
                m_keyboardHandler.setKeyPress(8);
                break;

            case sf::Keyboard::D:
                m_keyboardHandler.setKeyPress(9);
                break;

            case sf::Keyboard::F:
                m_keyboardHandler.setKeyPress(14);
                break;

            case sf::Keyboard::Z:
                m_keyboardHandler.setKeyPress(10);
                break;

            case sf::Keyboard::X:
                m_keyboardHandler.setKeyPress(0);
                break;

            case sf::Keyboard::C:
                m_keyboardHandler.setKeyPress(11);
                break;

            case sf::Keyboard::V:
                m_keyboardHandler.setKeyPress(15);
                break;

            default:
                break;
            }
        }
    }

    return true;
}

void Renderer::render(std::array<bool, 2048>& displayBuffer) {
    std::vector<sf::RectangleShape> boxes;
    int pos = 0;

    m_window.clear(sf::Color::Black);

    for (auto &i : displayBuffer) {
        if(i == true) {
            m_window.draw(m_boxes[pos]);
        }

        pos++;
    }

    m_window.display();
}
