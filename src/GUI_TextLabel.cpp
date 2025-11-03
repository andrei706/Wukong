
#include "GUI_TextLabel.h"

GUI_TextLabel::GUI_TextLabel(sf::Text TextValue_, const std::string &Name_, const std::string &FontPath, int TextSize,
    sf::Color TextColor): TextValue(TextValue_), Name(Name_) {
    if (!TextFont.openFromFile(FontPath)) {
        std::cout<<"Error: Font not found for Text Label, must give the path to font_name.ttf.";
    }
    TextValue.setFont(TextFont);
    TextValue.setCharacterSize(TextSize);
    TextValue.setFillColor(TextColor);
}

void GUI_TextLabel::SetText(std::string TextValue_) {
    TextValue.setString(TextValue_);
}

void GUI_TextLabel::SetPosition(sf::Vector2f Position_) {
    Position = Position_;
    TextValue.setPosition(Position_);
}

void GUI_TextLabel::SetColor(sf::Color TextColor) {
    TextValue.setFillColor(TextColor);
}

void GUI_TextLabel::SetSize(int Size_) {
    TextValue.setCharacterSize(Size_);
}

void GUI_TextLabel::SetName(const std::string &Name_) {
    Name = Name_;
}

const std::string & GUI_TextLabel::GetName() const {
    return Name;
}

bool GUI_TextLabel::GetStatus() const {
    return Status;
}

void GUI_TextLabel::ToggleActive() {
    Status = !Status;
}

void GUI_TextLabel::ShowSprite(sf::RenderWindow &window) const {
    window.draw(TextValue);
}

std::ostream & operator<<(std::ostream &out, const GUI_TextLabel &object) {
    out<<object.Name<<"\nStatus: "<<object.Status<<"\n";
    return out;
}
