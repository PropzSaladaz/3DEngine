#pragma once

#include <cmath>
#include "math/vector.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp> // glm::angle

namespace mgl::math {

    template<typename T>
    inline T radians(T degrees) noexcept {
        return degrees * (static_cast<T>(M_PI) / static_cast<T>(180));
    }

    template<int N, typename T>
    inline T angle(const vec_t<N, T>& vec1, const vec_t<N, T>& vec2) noexcept {
        return glm::angle(vec1.backend(), vec2.backend());
    }

    // ------------------------------------------------------------
    // Trigonometric functions
    // ------------------------------------------------------------
    
    template<typename T>
    inline T cos(T angleRadians) noexcept {
        return std::cos(angleRadians);
    }
    template<typename T>
    inline T sin(T angleRadians) noexcept {
        return std::sin(angleRadians);
    }
    template<typename T>
    inline T tan(T angleRadians) noexcept {
        return std::tan(angleRadians);
    }
    template<typename T>
    inline T acos(T value) noexcept {
        return std::acos(value);
    }

}
