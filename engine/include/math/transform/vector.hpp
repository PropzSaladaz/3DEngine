#pragma once

#include "math/vector.hpp"

namespace mgl::math {

    template<int N, typename T>
    inline vec_t<N, T> normalize(const vec_t<N, T>& v) noexcept {
        return v.normalized();
    }

    template<int N, typename T>
    constexpr T dot(const vec_t<N,T>& a, const vec_t<N,T>& b) noexcept {
        return dot(a,b); // call friend
    }

    template<typename T>
    constexpr vec_t<3,T> cross(const vec_t<3,T>& a, const vec_t<3,T>& b) noexcept {
        return cross(a,b); // call friend
    }

    template<int N, typename T>
    constexpr vec_t<N,T> lerp(const vec_t<N,T>& a, const vec_t<N,T>& b, T t) noexcept {
        return lerp(a,b,t); // call friend
    }

}