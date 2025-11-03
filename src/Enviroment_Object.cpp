

#include "Enviroment_Object.h"

Enviroment_Object::Enviroment_Object(bool isDestructable_, float Health_): isDestructable(isDestructable_), Health(Health_) {}

std::ostream & operator<<(std::ostream &out, const Enviroment_Object &object) {
    out<<object.isDestructable<<" "<<object.Health;
    return out;
}
