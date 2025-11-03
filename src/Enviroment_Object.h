
#ifndef OOP_ENVIROMENT_OBJECT_H
#define OOP_ENVIROMENT_OBJECT_H

#include <iostream>


class Enviroment_Object {
    bool isDestructable;
    float Health;
public:
    Enviroment_Object(bool isDestructable_, float Health_);

    friend std::ostream & operator<<(std::ostream & out, const Enviroment_Object & object);
};

#endif //OOP_ENVIROMENT_OBJECT_H