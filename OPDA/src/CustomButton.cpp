#include "CustomButton.h"

CustomButton::CustomButton(const sf::Vector2f& btnSize, const sf::Vector2f& btnPos, std::function<void()> clickFunc, const std::wstring& str, const int& charSize)
{
    this->setPosition(btnPos);
    this->setSize(btnSize);

    this->text.setFont(sv::Fonts::arial);
    this->text.setCharacterSize(charSize);

    this->text.setString(str);
    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    this->text.setPosition(btnPos + 0.5f * btnSize);
    // Установка цвета текста
    this->text.setFillColor(sf::Color::White);

    this->setFillColor(sf::Color(128, 128, 128, 128));

    this->clickFunc = clickFunc;
}

void CustomButton::setButtonColor(const sf::Color& color)
{
    this->setFillColor(color);
}

void CustomButton::setTextColor(const sf::Color& color)
{
    this->text.setFillColor(color);
}

bool CustomButton::isMouseOver(const sf::RenderWindow& window) const
{
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    return this->getGlobalBounds().contains(mousePos);
}

void CustomButton::draw(sf::RenderWindow& window) const
{
    if (visible)
    {
        window.draw(*this);
        window.draw(this->text);
    }
}

void CustomButton::update(const sf::RenderWindow& window, const sf::Event& event)
{
    if (enabled)
    {
        if (isMouseOver(window))
        {
            if (!flags.isMouseOver)
            {

                this->setFillColor(sf::Color(this->getFillColor().r / 2, this->getFillColor().g / 2, this->getFillColor().b / 2));
            }
            if (event.type == sf::Event::MouseButtonPressed && !flags.isClicked)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    flags.isClicked = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased && flags.isClicked)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    flags.isClicked = false;

                    if (clickFunc != nullptr)
                    {
                        clickFunc();
                    }
                }
            }
            flags.isMouseOver = true;
        }
        else
        {
            if (flags.isMouseOver && !flags.isClicked)
            {

                this->setFillColor(sf::Color(this->getFillColor().r * 2, this->getFillColor().g * 2, this->getFillColor().b * 2));
            }
            if (event.type == sf::Event::MouseButtonReleased && flags.isClicked)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    flags.isClicked = false;
                    this->setFillColor(sf::Color(this->getFillColor().r * 2, this->getFillColor().g * 2, this->getFillColor().b * 2));
                }
            }

            flags.isMouseOver = false;
        }
    }
}
