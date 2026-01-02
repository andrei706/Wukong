#include "Enviroment_Object.h"

Enviroment_Object::Enviroment_Object(sf::Vector2f pos, sf::Vector2f size,
                                     const std::string& texturePath){
    if (!Texture.loadFromFile(texturePath)) {
        throw AssetMissingException(texturePath);
    }

    Sprite.setPosition(pos);
    Sprite.setSize(size);
    Sprite.setTexture(&Texture);
}

void Enviroment_Object::ShowSprite(sf::RenderWindow& window) const {
    window.draw(Sprite);
}

std::ostream & operator<<(std::ostream & out, const Enviroment_Object & object) {
    out << "Block position: " << object.Sprite.getPosition().x << ", " << object.Sprite.getPosition().y;
    return out;
}