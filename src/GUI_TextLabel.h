
#ifndef OOP_GUI_TEXTLABEL_H
#define OOP_GUI_TEXTLABEL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>


class GUI_TextLabel {
    sf::Font TextFont;
    sf::Text TextValue;
    sf::Vector2f Position = {100.f, 100.f};

    std::string Name;
    bool Status = true;
public:
    explicit GUI_TextLabel(sf::Text TextValue_,const std::string& Name_ = "TextLabel",  const std::string& FontPath = "data/fonts/arial.ttf", int TextSize = 10, sf::Color TextColor = sf::Color::Black);

    friend std::ostream & operator<<(std::ostream & out, const GUI_TextLabel & object);

    void SetText(std::string TextValue_);

    void SetPosition(sf::Vector2f Position_);

    void SetColor(sf::Color TextColor);

    void SetSize(int Size_);

    void SetName(const std::string & Name_);

    const std::string& GetName() const;

    bool GetStatus() const;

    void ToggleActive();

    void ShowSprite(sf::RenderWindow& window) const;
};


#endif //OOP_GUI_TEXTLABEL_H