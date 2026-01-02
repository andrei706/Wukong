
#ifndef OOP_RANDOM_VALUE_GENERATOR_H
#define OOP_RANDOM_VALUE_GENERATOR_H

#include <random>
#include <iostream>
#include <ostream>

template <typename T>
class Random_Value_Generator {
    T min_val;
    T max_val;
    std::mt19937 generator;

public:
    Random_Value_Generator(T min, T max);
    T generate();
    friend std::ostream & operator<<(std::ostream &os, const Random_Value_Generator &obj) {
        return os
               << "min_val: " << obj.min_val
               << " max_val: " << obj.max_val;
    }
};

#endif //OOP_RANDOM_VALUE_GENERATOR_H
