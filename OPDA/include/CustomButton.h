#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <wchar.h>
#include <iostream>
#include <functional>
#include "StaticVariables.h"

struct ButtonFlags
{
    ButtonFlags() :
        isMouseOver(false),
        isClicked(false)
    {}
    bool isMouseOver;
    bool isClicked;
};
class CustomButton : public sf::RectangleShape {
public:
    ButtonFlags flags;
    CustomButton(const sf::Vector2f& btnSize, const sf::Vector2f& btnPos, std::function<void()> clickFunc = nullptr, const std::wstring& str = L"", const int& charSize = 24);

    void setButtonColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);

    bool isMouseOver(const sf::RenderWindow& window) const;
    void draw(sf::RenderWindow& window) const;
    void update(const sf::RenderWindow& window, const sf::Event& event);
    sf::Text text;
    std::function<void()> clickFunc;

    void setEnabled(bool enabled) { this->enabled = enabled; }
    void setVisible(bool visible) { this->visible = visible; }

private:
    bool enabled = true;
    bool visible = true;
};
