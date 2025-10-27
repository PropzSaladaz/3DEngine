// vec_t.hpp — fixed-size row-style vectors with GLM backend hidden
#pragma once
#include "config.hpp"
#include <glm/glm.hpp>
#include <type_traits>
#include <array>
#include <cmath>
#include <algorithm>

namespace mgl::math {

// Forward-declare matrix to enable mat*vec_t / vec_t*mat
template<int R, int C, typename T> struct mat;
template<typename T> struct quat_t;

// ==================================================
// Vector of size N with scalar type T (default: float)
// ==================================================

template<int N, typename T = scalar>
struct vec_t {
    static_assert(N > 0, "vector: N must be positive");
    using scalar_type = T;
    static constexpr int size = N;

    using backend_t = glm::vec<N, T, glm::defaultp>;
private:
    backend_t v;

    // lets quaternion access v directly
    template<typename U> friend struct quat_t;

public:
    // --- ctors ---
    constexpr vec_t() : v(0) {}                // zero initialize
    explicit constexpr vec_t(const backend_t& g) : v(g) {}

    // splat
    explicit constexpr vec_t(T s) : v(s) {}

    // N-scalar constructor (row order)
    template<typename... Args>
    requires (sizeof...(Args) == N) && (std::is_convertible_v<Args,T> && ...)
    constexpr vec_t(Args... vals) : v( static_cast<T>(vals)... ) {}

    // from array
    explicit constexpr vec_t(const std::array<T,N>& a) : v(0) {
        for (int i=0;i<N;++i) v[i] = a[i];
    }

    // access
    constexpr       T& operator[](int i)       noexcept { return v[i]; }
    constexpr const T& operator[](int i) const noexcept { return v[i]; }

    // named accessors (enabled only when valid)
    constexpr       T& x()       noexcept requires (N>=1) { return v[0]; }
    constexpr const T& x() const noexcept requires (N>=1) { return v[0]; }
    constexpr       T& y()       noexcept requires (N>=2) { return v[1]; }
    constexpr const T& y() const noexcept requires (N>=2) { return v[1]; }
    constexpr       T& z()       noexcept requires (N>=3) { return v[2]; }
    constexpr const T& z() const noexcept requires (N>=3) { return v[2]; }
    constexpr       T& w()       noexcept requires (N>=4) { return v[3]; }
    constexpr const T& w() const noexcept requires (N>=4) { return v[3]; }

    // data
    constexpr       T* data()       noexcept { return &v[0]; }
    constexpr const T* data() const noexcept { return &v[0]; }

    // unary
    constexpr vec_t operator+() const noexcept { return *this; }
    constexpr vec_t operator-() const noexcept {
        vec_t r;
        for (int i=0;i<N;++i) r[i] = -v[i];
        return r;
    }

    // element-wise +, -, *, /
    friend constexpr vec_t operator+(const vec_t& a, const vec_t& b) noexcept {
        vec_t r; for (int i=0;i<N;++i) r[i] = a[i] + b[i]; return r;
    }
    friend constexpr vec_t operator-(const vec_t& a, const vec_t& b) noexcept {
        vec_t r; for (int i=0;i<N;++i) r[i] = a[i] - b[i]; return r;
    }
    friend constexpr vec_t operator*(const vec_t& a, const vec_t& b) noexcept {
        vec_t r; for (int i=0;i<N;++i) r[i] = a[i] * b[i]; return r;
    }
    friend constexpr vec_t operator/(const vec_t& a, const vec_t& b) noexcept {
        vec_t r; for (int i=0;i<N;++i) r[i] = a[i] / b[i]; return r;
    }

    // scalar ops (vec_t ∘ scalar)
    friend constexpr vec_t operator*(const vec_t& a, T s) noexcept {
        vec_t r; for (int i=0;i<N;++i) r[i] = a[i] * s; return r;
    }
    friend constexpr vec_t operator*(T s, const vec_t& a) noexcept { return a * s; }
    friend constexpr vec_t operator/(const vec_t& a, T s) noexcept {
        vec_t r; for (int i=0;i<N;++i) r[i] = a[i] / s; return r;
    }

    // compound
    constexpr vec_t& operator+=(const vec_t& b) noexcept { for (int i=0;i<N;++i) v[i]+=b[i]; return *this; }
    constexpr vec_t& operator-=(const vec_t& b) noexcept { for (int i=0;i<N;++i) v[i]-=b[i]; return *this; }
    constexpr vec_t& operator*=(T s)          noexcept { for (int i=0;i<N;++i) v[i]*=s;   return *this; }
    constexpr vec_t& operator/=(T s)          noexcept { for (int i=0;i<N;++i) v[i]/=s;   return *this; }

    // exact equality
    friend constexpr bool operator==(const vec_t& a, const vec_t& b) noexcept {
        for (int i=0;i<N;++i) if (!(a[i] == b[i])) return false;
        return true;
    }
    friend constexpr bool operator!=(const vec_t& a, const vec_t& b) noexcept { return !(a==b); }

    // length squared
    constexpr T length2() const noexcept {
        T s = 0; 
        for (int i=0;i<N;++i) s += v[i]*v[i]; 
        return s;
    }

    T length() const noexcept { using std::sqrt; return sqrt(length2()); }

    vec_t normalized() const noexcept {
        T len = length();
        // Avoid div-by-zero: return zero if len == 0
        if (len == T(0)) return vec_t{};
        return *this / len;
    }

    // dot
    friend constexpr T dot(const vec_t& a, const vec_t& b) noexcept {
        T s = 0; for (int i=0;i<N;++i) s += a[i]*b[i]; return s;
    }

    // cross (only for N==3): standard 3D cross product
    friend constexpr vec_t<3,T> cross(const vec_t<3,T>& a, const vec_t<3,T>& b) noexcept
        requires (N==3)
    {
        return vec_t<3,T>(
            a[1]*b[2] - a[2]*b[1],
            a[2]*b[0] - a[0]*b[2],
            a[0]*b[1] - a[1]*b[0]
        );
    }

    // 2D "scalar cross": returns z-component of 3D cross with z=0
    friend constexpr T crossZ(const vec_t<2,T>& a, const vec_t<2,T>& b) noexcept
        requires (N==2)
    {
        return a[0]*b[1] - a[1]*b[0];
    }
};


// make friend functions visible and scoped to math namespace
template<int N, typename T>
constexpr T dot(const vec_t<N,T>& a, const vec_t<N,T>& b) noexcept {
    return dot(a,b); // call friend
}

template<typename T>
constexpr vec_t<3,T> cross(const vec_t<3,T>& a, const vec_t<3,T>& b) noexcept {
    return cross(a,b); // call friend
}

// ------------------------- approx equality helper -------------------------

template<int N, typename T>
inline bool approxEqual(const vec_t<N,T>& a, const vec_t<N,T>& b,
                         T abs_eps = math::abs_eps, T rel_eps = math::rel_eps) noexcept
{
    for (int i=0;i<N;++i) {
        const T x = a[i], y = b[i];
        const T diff = std::abs(x - y);
        const T tol  = std::max(abs_eps, rel_eps * std::max(std::abs(x), std::abs(y)));
        if (diff > tol) return false;
    }
    return true;
}

// -------------------------- mat × vec_t / vec_t × mat --------------------------
// (R×C) * (C) → (R)
template<int R, int C, typename Tm, typename Tv>
constexpr vec_t<R, std::common_type_t<Tm,Tv>>
operator*(const mat<R,C,Tm>& M, const vec_t<C,Tv>& x)
{
    using T = std::common_type_t<Tm,Tv>;
    vec_t<R,T> y;
    for (int r=0;r<R;++r) {
        T acc = 0;
        for (int c=0;c<C;++c) acc += static_cast<T>(M(r,c)) * static_cast<T>(x[c]);
        y[r] = acc;
    }
    return y;
}

// (R) * (R×C) → (C)   // row-vector times matrix
template<int R, int C, typename Tv, typename Tm>
constexpr vec_t<C, std::common_type_t<Tv,Tm>>
operator*(const vec_t<R,Tv>& x, const mat<R,C,Tm>& M)
{
    using T = std::common_type_t<Tv,Tm>;
    vec_t<C,T> y;
    for (int c=0;c<C;++c) {
        T acc = 0;
        for (int r=0;r<R;++r) acc += static_cast<T>(x[r]) * static_cast<T>(M(r,c));
        y[c] = acc;
    }
    return y;
}

// ------------------------------ handy aliases ------------------------------

template<typename T = scalar> using vec2_t = vec_t<2,T>;
template<typename T = scalar> using vec3_t = vec_t<3,T>;
template<typename T = scalar> using vec4_t = vec_t<4,T>;

using vec2 = vec2_t<float>;
using vec3 = vec3_t<float>;
using vec4 = vec4_t<float>;

using vec2d = vec2_t<double>;
using vec3d = vec3_t<double>;
using vec4d = vec4_t<double>;

using vec2i = vec2_t<int>;
using vec3i = vec3_t<int>;
using vec4i = vec4_t<int>;

using vec2u = vec2_t<unsigned>;
using vec3u = vec3_t<unsigned>;
using vec4u = vec4_t<unsigned>;

using vec2ui = vec2_t<std::uint32_t>;
using vec3ui = vec3_t<std::uint32_t>;
using vec4ui = vec4_t<std::uint32_t>;

} // namespace mgl::math
