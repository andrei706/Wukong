
#include "Template_Function.cpp"
#include "Enemy.h"
#include "Enviroment_Object.h"

template void RenderCollection<Enemy>(sf::RenderWindow&, const std::vector<std::shared_ptr<Enemy>>&);
template void RenderCollection<Enviroment_Object>(sf::RenderWindow&, const std::vector<std::shared_ptr<Enviroment_Object>>&);