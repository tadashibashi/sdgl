#pragma once
#include <type_traits>

namespace sdgl {
    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;
}
