#include "Template_Function.h"

template <typename T>
void RenderCollection(sf::RenderWindow& window, const std::vector<std::shared_ptr<T>>& collection) {
    for (const auto& item : collection) {
        if (item) {
            item->ShowSprite(window);
        }
    }
}