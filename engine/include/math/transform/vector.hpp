#pragma once

#include "math/vector.hpp"

namespace mgl::math {

    template<int N, typename T>
    inline vec_t<N, T> normalize(const vec_t<N, T>& v) noexcept {
        return v.normalized();
    }

    template<int N, typename T>
    constexpr T dot(const vec_t<N,T>& a, const vec_t<N,T>& b) noexcept {
        return glm::dot(a.backend(), b.backend());
    }

    template<typename T>
    constexpr vec_t<3,T> cross(const vec_t<3,T>& a, const vec_t<3,T>& b) noexcept {
        auto r = glm::cross(a.backend(), b.backend());
        return vec_t<3,T>(r.x, r.y, r.z);
    }

    template<int N, typename T>
    constexpr vec_t<N,T> lerp(const vec_t<N,T>& a, const vec_t<N,T>& b, T t) noexcept {
        return a + t * (b - a);
    }

}
