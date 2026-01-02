
#include "GUI_TextLabel.h"

GUI_TextLabel::GUI_TextLabel(const std::string &Name_, const std::string &FontPath, int TextSize, sf::Color TextColor, bool OutlineEnabled)
: TextFont(std::make_shared<sf::Font>()),
  TextValue(*TextFont),
  Name(Name_)
    {
    if (!TextFont->openFromFile(FontPath)) {
        throw AssetMissingException(FontPath);
    }
    TextValue.setCharacterSize(TextSize);
    TextValue.setFillColor(TextColor);
    if (OutlineEnabled) {
        TextValue.setOutlineColor(sf::Color::Black);
        TextValue.setOutlineThickness(2.0f);
    }
}

GUI_TextLabel::GUI_TextLabel(const GUI_TextLabel& other)
    : TextFont(other.TextFont), TextValue(other.TextValue),
      Position(other.Position), Name(other.Name), Status(other.Status) {
    TextValue.setFont(*TextFont);
}

GUI_TextLabel& GUI_TextLabel::operator=(const GUI_TextLabel& other) {
    if (this != &other) {
        TextFont = other.TextFont;
        TextValue = other.TextValue;
        Position = other.Position;
        Name = other.Name;
        Status = other.Status;
        TextValue.setFont(*TextFont);
    }
    return *this;
}

void GUI_TextLabel::SetText(const std::string &TextValue_) {
    TextValue.setString(TextValue_);
}

void GUI_TextLabel::SetPosition(sf::Vector2f Position_) {
    Position = Position_;
    TextValue.setPosition(Position_);
}

// void GUI_TextLabel::SetColor(sf::Color TextColor) {
//     TextValue.setFillColor(TextColor);
// }

// void GUI_TextLabel::SetSize(int Size_) {
//     TextValue.setCharacterSize(Size_);
// }

const std::string & GUI_TextLabel::GetName() const {
    return Name;
}

bool GUI_TextLabel::GetStatus() const {
    return Status;
}

void GUI_TextLabel::ToggleActive() {
    Status = !Status;
}

sf::Vector2f GUI_TextLabel::GetPosition() const {
    return TextValue.getPosition();
}

void GUI_TextLabel::ShowSprite(sf::RenderWindow &window) const {
    window.draw(TextValue);
}

std::ostream & operator<<(std::ostream &out, const GUI_TextLabel &object) {
    out<<object.Name<<"\nStatus: "<<object.Status<<"\n";
    return out;
}
