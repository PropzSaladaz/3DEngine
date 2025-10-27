#pragma once
#include <cmath>
#include <algorithm>

namespace mgl::test {

// default tolerances for approximate comparisons

// utility to compare vectors for approximate equality
template<int N, typename T, template<int, typename> class VecT>
inline bool approx_vec(const VecT<N, T>& a,
                       const VecT<N, T>& b,
                       T abs_eps = T(kAbsEps),
                       T rel_eps = T(kRelEps)) noexcept
{
    for (int i = 0; i < N; ++i) {
        const T x = a[i], y = b[i];
        const T diff = std::abs(x - y);
        const T tol = std::max(abs_eps, rel_eps * std::max(std::abs(x), std::abs(y)));
        if (diff > tol) return false;
    }
    return true;
}

} // namespace mgl::test
