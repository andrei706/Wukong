
#ifndef OOP_GUI_TEXTLABEL_H
#define OOP_GUI_TEXTLABEL_H

#include "Game_Exceptions.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <memory>

class GUI_TextLabel {
    std::shared_ptr<sf::Font> TextFont;
    sf::Text TextValue;
    sf::Vector2f Position = {100.f, 100.f};
    std::string Name;
    bool Status = true;
public:
    explicit GUI_TextLabel(const std::string& Name_ = "TextLabel",
                           const std::string& FontPath = "data/fonts/arial.ttf",
                           int TextSize = 10,
                           sf::Color TextColor = sf::Color::Black,
                           bool OutlineEnabled = false);

    GUI_TextLabel(const GUI_TextLabel& other);
    GUI_TextLabel& operator=(const GUI_TextLabel& other);

    friend std::ostream & operator<<(std::ostream & out, const GUI_TextLabel & object);
    void SetText(const std::string &TextValue_);
    void SetPosition(sf::Vector2f Position_);
    void SetColor(sf::Color TextColor);
    //void SetSize(int Size_);
    const std::string& GetName() const;
    bool GetStatus() const;
    void ToggleActive();
    sf::Vector2f GetPosition() const;
    void ShowSprite(sf::RenderWindow& window) const;
};


#endif //OOP_GUI_TEXTLABEL_H