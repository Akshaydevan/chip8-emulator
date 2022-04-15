#include "keyboardhandler.hpp"

KeyboardHandler& KeyboardHandler::getInstance() {
    static KeyboardHandler instance;

    return instance;
}

void KeyboardHandler::setKeyPress(int key) {
    m_keyPress.push_back(key);
}

int KeyboardHandler::getKeyPress() {
    if (m_keyPress.size() - index > 0){
        return m_keyPress[index++];
    }
    else
        return -1;
}
