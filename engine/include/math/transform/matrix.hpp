#pragma once
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include <glm/gtc/matrix_transform.hpp> // glm::scale

namespace mgl::math {

    template<typename T>
    inline mat_t<4, 4, T> scale(const mat_t<4, 4, T>& m, vec_t<3, T> scale) noexcept {
        return mat_t<4, 4, T>(glm::scale(m.backend(), scale.backend()));
    }

    template<typename T>
    inline mat_t<4, 4, T> translate(const mat_t<4, 4, T>& m, vec_t<3, T> translation) noexcept {
        return mat_t<4, 4, T>(glm::translate(m.backend(), translation.backend()));
    }

    template<typename T>
    inline mat_t<4, 4, T> lookAt(const vec_t<3, T>& eye, const vec_t<3, T>& target, const vec_t<3, T>& up) noexcept {
        return mat_t<4, 4, T>(glm::lookAt(eye.backend(), target.backend(), up.backend()));
    }

    template<typename T>
    inline mat_t<4, 4, T> perspective(T fovY, T aspect, T near, T far) noexcept {
        return mat_t<4, 4, T>(glm::perspective(fovY, aspect, near, far));
    }
}