#pragma once

namespace mgl::math {

    using scalar = float; // flip to double if you ever need it
    constexpr scalar epsilon = 1e-8;
    constexpr scalar abs_eps = 1e-6;
    constexpr scalar rel_eps = 1e-5;
}