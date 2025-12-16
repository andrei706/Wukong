
#include "Attack_Projectile.h"

Attack_Projectile::Attack_Projectile(
    float DamageValue_,
    sf::Vector2f Size_,
    sf::Vector2f Position_,
    sf::Angle Angle_,
    sf::Vector2f Velocity_,
    float Lifetime_,
    float Cooldown_)
: Attack_Hitbox(DamageValue_, Size_, Position_, Angle_, Lifetime_, Cooldown_, "data/textures/attack_assets/projectile_sprite.png"),
  Velocity(Velocity_)
{
    Sprite.setSize(Size_);
    Sprite.setOrigin({Size_.x / 2, Size_.y / 2});
    Sprite.setPosition(Position_);
    Sprite.setRotation(Angle_);

    Sprite.setFillColor(sf::Color::Yellow);
}

void Attack_Projectile::UpdateBehavior(float deltaTime) {
    if (Lifetime > 0) {
        Position += Velocity * deltaTime * 100.f;
        Sprite.setPosition(Position);
        Lifetime -= deltaTime;
    }
    if (Lifetime <= 0) {
        isActive = false;
    }
}
