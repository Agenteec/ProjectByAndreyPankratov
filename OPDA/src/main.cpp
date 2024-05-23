#include <iostream>
#include "Window.h"

int main()
{
    sv::Fonts::Init();
    Window* window = new Window();
    window->open();
    delete window;
}