#pragma once

#include "types.hpp"
#include <cstdint>

namespace mgl::math {

using scalar = float; // flip to double if you ever need it
constexpr scalar abs_eps = 1e-6;
constexpr scalar rel_eps = 1e-5;

// Mathematical constants
constexpr scalar PI = 3.14159265358979323846f;
constexpr scalar TWO_PI = PI * 2.0f;
constexpr scalar HALF_PI = PI / 2.0f;

template <typename T> constexpr T epsilon() noexcept {
  return static_cast<T>(abs_eps);
}

} // namespace mgl::math