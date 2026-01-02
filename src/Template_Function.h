
#ifndef OOP_TEMPLATE_FUNCTION_H
#define OOP_TEMPLATE_FUNCTION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

template <typename T>
void RenderCollection(sf::RenderWindow& window, const std::vector<std::shared_ptr<T>>& collection);

#endif //OOP_TEMPLATE_FUNCTION_H