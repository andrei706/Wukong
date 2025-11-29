
#include "Attack_Warning.h"

Attack_Warning::Attack_Warning(bool visible_, float VisibilityTime_, const std::string &TexturePath_):   Visible(visible_), VisibilityTime(VisibilityTime_), TexturePath(TexturePath_) {
    if (!Texture.loadFromFile(TexturePath_)) {
        std::cerr << "Error: Could not load texture from " << TexturePath_ << std::endl;
    }
    Sprite.setTexture(&Texture);

    sf::Vector2f Size = static_cast<sf::Vector2f>(Texture.getSize());
    Size = {Size.x / 1.5f, Size.y / 1.5f};
    Sprite.setSize(Size);
    Sprite.setOrigin({Size.x/2, Size.y/2});
}

void Attack_Warning::SetPosition(sf::Vector2f position_) {
    Sprite.setPosition(position_);
}

void Attack_Warning::ShowSprite(sf::RenderWindow &window) const {
    if (Visible == true) {
        window.draw(Sprite);
    }
}

void Attack_Warning::Update() {
    if (Timer.getElapsedTime().asSeconds() >= VisibilityTime) {
        Visible = false;
    }
}

void Attack_Warning::SetVisibility(bool value, float VisibilityTime_) {
    Visible = value;
    VisibilityTime = VisibilityTime_;
    if (Visible == true) {
        Timer.restart();
    }
}
