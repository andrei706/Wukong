
#include "Random_Value_Generator.h"

template<typename T>
Random_Value_Generator<T>::Random_Value_Generator(T min, T max) : min_val(min), max_val(max) {
    std::random_device rd;
    generator.seed(rd());
}

template<typename T>
T Random_Value_Generator<T>::generate() {
    if constexpr (std::is_integral<T>::value) {
        std::uniform_int_distribution<T> dist(min_val, max_val);
        return dist(generator);
    } else {
        std::uniform_real_distribution<T> dist(min_val, max_val);
        return dist(generator);
    }
}