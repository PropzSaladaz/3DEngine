#pragma once

#include "math/quaternion.hpp"
#include "math/vector.hpp"

namespace mgl::math {

    // Fast path: expects a and b are unit-length (|a|=|b|=1).
    // Returns a unit quaternion rotating a -> b (shortest arc).
    template<typename T>
    quat_t<T> rotationUnit(const vec_t<3, T>& a, const vec_t<3, T>& b) {
        return quat_t<T>(glm::rotation(a.backend(), b.backend()));
    }

    template<typename T>
    vec_t<3, T> rotate(const quat_t<T>& q, vec_t<3, T>& input) {
        return q * input;
    }

    template<typename T>
    quat_t<T> slerp(const quat_t<T>& a, const quat_t<T>& b, T t) {
        return slerp(a, b, t);
    }

    template<typename T>
    quat_t<T> normalize(const quat_t<T>& q) {
        return q.normalized();
    }
}