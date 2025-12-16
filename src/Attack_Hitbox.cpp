
#include "Attack_Hitbox.h"

#include "Enemy.h"

Attack_Hitbox::Attack_Hitbox(float DamageValue_, sf::Vector2f Size_, sf::Vector2f Position_, sf::Angle Angle_, float Lifetime_, float Cooldown_,
                             const std::string &TexturePath)
    : DamageValue(DamageValue_), Size(Size_), Position(Position_), Rotation(Angle_), Lifetime(Lifetime_), Cooldown(Cooldown_) {

    if (!Texture.loadFromFile(TexturePath)) {
        std::cerr << "Eroare la incarcarea texturii slash!" << std::endl;
    }

    Sprite.setTexture(&Texture);

    Sprite.setSize(Size_);
    Sprite.setOrigin({Size_.x / 2, Size_.y / 2});
    Sprite.setPosition(Position_);
    Sprite.setRotation(Angle_);
    Sprite.setFillColor(sf::Color::White);
}



float Attack_Hitbox::GetDamageValue(int EnemyId){
    if (EnemyId == -1) return DamageValue; // This is the player ID
    if (!canDamage) return 0;

    for (auto const &entry : Attacked) {
        if (entry.first == EnemyId) {
            return 0;;
        }
    }

    Attacked.emplace_back(EnemyId, 0.5f);
    for (auto const &entry : Attacked) {
        std::cout << entry.first << " " << entry.second << std::endl;
    }

    return DamageValue;
}

sf::FloatRect Attack_Hitbox::GetBounds() const {
    return Sprite.getGlobalBounds();
}

void Attack_Hitbox::ShowSprite(sf::RenderWindow &window) const {
    window.draw(Sprite);
}

void Attack_Hitbox::UpdateBehavior(float deltaTime) {
    sf::Color currentColor = Sprite.getFillColor();

    float deltaAlpha = 600.0f * deltaTime;
    int newAlpha = (int)(currentColor.a - deltaAlpha);
    if (newAlpha < 0) {
        newAlpha = 0;
    }
    sf::Color newColor(currentColor.r, currentColor.g, currentColor.b, newAlpha);

    Sprite.setFillColor(newColor);

    if (Lifetime <= 0) {
        canDamage = false;
        isActive = false;
    }
    Lifetime -= deltaTime;
}

bool Attack_Hitbox::IsActive() const {
    return isActive;
}

bool Attack_Hitbox::Update(float deltaTime) {
    UpdateBehavior(deltaTime);
    // for (int i = Attacked.size() - 1; i >= 0; i--) {
    //     Attacked[i].second -= deltaTime;
    //     if (Attacked[i].second <= 0) {
    //         Attacked.erase(Attacked.begin() + i);
    //     }
    // }
    return isActive;
}

std::ostream & operator<<(std::ostream &out, const Attack_Hitbox &object) {
    out<<object.DamageValue<<"\n";
    return out;
}
