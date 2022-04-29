#pragma once

#include <vector>

class KeyboardHandler {
public:
    KeyboardHandler(KeyboardHandler&) = delete;

    static KeyboardHandler& getInstance();
    void setKeyPress(int);
    int getKeyPress();

private:
    KeyboardHandler() = default;

    int m_key = -1;
};
