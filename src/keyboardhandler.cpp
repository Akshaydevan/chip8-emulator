#include "keyboardhandler.hpp"

KeyboardHandler& KeyboardHandler::getInstance()
{
    static KeyboardHandler instance;

    return instance;
}

void KeyboardHandler::setKeyPress(int key)
{
    m_key = key;
}

int KeyboardHandler::getKeyPress()
{
    return m_key;
}
