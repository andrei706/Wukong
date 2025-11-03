
#include "Attack_Hitbox.h"

Attack_Hitbox::Attack_Hitbox(float DamageValue_, sf::Vector2f Size_, sf::Vector2f Position_, sf::Angle Angle_): DamageValue(DamageValue_), Size(Size_), Position(Position_), Rotation(Angle_) {
    Sprite.setSize(Size_);
    Sprite.setOrigin({Size_.x / 2, Size_.y / 2});
    Sprite.setPosition(Position_);
    Sprite.setRotation(Angle_);

    Sprite.setFillColor(sf::Color::Magenta);

}

float Attack_Hitbox::GetDamageValue() const {
    return DamageValue;
}

sf::FloatRect Attack_Hitbox::GetBounds() const {
    return Sprite.getGlobalBounds();
}

void Attack_Hitbox::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
}

std::ostream & operator<<(std::ostream &out, const Attack_Hitbox &object) {
    out<<object.DamageValue<<"\n";
    return out;
}
