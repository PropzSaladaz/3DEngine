// quat_t.hpp — thin, backend-agnostic quaternion wrapper (snake_case API)
// - Zero-cost wrapper over GLM's glm::quat_t (assumed default layout: w,x,y,z).
// - Public accessors follow XYZW order: q.x(), q.y(), q.z(), q.w().
// - Named constructors + raw ctor (x,y,z,w).
// - rotate(vec3) only; GLM composition order: (q1*q2) applies q2, then q1.
#pragma once
#include "config.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>   // glm::quat_t, angleAxis, normalize, conjugate
#include <glm/gtx/quaternion.hpp>   // glm::toMat3/4 (not used), slerp, etc. (safe)
#include <type_traits>
#include <cmath>
#include "vector.hpp"
#include "matrix.hpp"

namespace mgl::math {

template<typename T = scalar>
struct quat_t {
    using scalar_type = T;
    using backend_t   = glm::qua<T, glm::defaultp>; // glm::quat_t alias

private:
    backend_t q_; // GLM stores (w,x,y,z). Accessors will present (x,y,z,w).

public:
    // ----------------------------- constructors -----------------------------

    constexpr quat_t(const backend_t& g) noexcept : q_(g) {} // backend ctor

    // Identity (unit) quaternion
    constexpr quat_t() noexcept : q_(T(1), T(0), T(0), T(0)) {}

    // Raw constructor (x,y,z,w) — different from GLM ordering (w,x,y,z) - matches to it internally
    explicit constexpr quat_t(T x, T y, T z, T w) noexcept : q_(w, x, y, z) {}
    explicit constexpr quat_t(const vec_t<3,T>& v, T w) noexcept : q_(w, v[0], v[1], v[2]) {}

    // Copy/move
    constexpr quat_t(const quat_t&) noexcept = default;
    constexpr quat_t(quat_t&&) noexcept = default;
    constexpr quat_t& operator=(const quat_t&) noexcept = default;
    constexpr quat_t& operator=(quat_t&&) noexcept = default;

    // ------------------------- named constructors ---------------------------

    // Identity
    static constexpr quat_t identity() noexcept { return quat_t(T(0), T(0), T(0), T(1)); }

    // From axis-angle (axis must be unit; angle in radians)
    template<int N = 3>
    static quat_t fromAxisAngle(const vec_t<N,T>& axis, T radians) noexcept {
        // glm::angleAxis expects (angle, axis)
        return quat_t(backend_t(glm::angleAxis(radians, glm::vec<3,T,glm::defaultp>(axis[0], axis[1], axis[2]))));
    }

    // From Euler angles (radians). Convention: pitch (X), yaw (Y), roll (Z).
    // Applied in the common GLM-friendly order: roll(Z) * yaw(Y) * pitch(X).
    static quat_t fromEuler(T pitch_x, T yaw_y, T roll_z) noexcept {
        backend_t qx = glm::angleAxis(pitch_x, glm::vec<3,T,glm::defaultp>(T(1),T(0),T(0)));
        backend_t qy = glm::angleAxis(yaw_y,   glm::vec<3,T,glm::defaultp>(T(0),T(1),T(0)));
        backend_t qz = glm::angleAxis(roll_z,  glm::vec<3,T,glm::defaultp>(T(0),T(0),T(1)));
        return quat_t(qz * qy * qx); // GLM order: rightmost applied first
    }

    // From small-axis helpers (radians)
    static quat_t fromX(T radians) noexcept { return quat_t(backend_t(glm::angleAxis(radians, glm::vec<3,T,glm::defaultp>(T(1),T(0),T(0))))); }
    static quat_t fromY(T radians) noexcept { return quat_t(backend_t(glm::angleAxis(radians, glm::vec<3,T,glm::defaultp>(T(0),T(1),T(0))))); }
    static quat_t fromZ(T radians) noexcept { return quat_t(backend_t(glm::angleAxis(radians, glm::vec<3,T,glm::defaultp>(T(0),T(0),T(1))))); }

    // -------------------------- accessors (xyzw) ----------------------------

    constexpr const backend_t& backend() const noexcept { return q_; }

    // access
    constexpr       T& operator[](int i)       noexcept { return q_[i]; }
    constexpr const T& operator[](int i) const noexcept { return q_[i]; }

    // Public accessor order: x,y,z,w (vector first, scalar last)
    constexpr       T& x()       noexcept { return q_.x; } // GLM provides .x component directly
    constexpr const T& x() const noexcept { return q_.x; }

    constexpr       T& y()       noexcept { return q_.y; }
    constexpr const T& y() const noexcept { return q_.y; }

    constexpr       T& z()       noexcept { return q_.z; }
    constexpr const T& z() const noexcept { return q_.z; }

    constexpr       T& w()       noexcept { return q_.w; } // scalar part
    constexpr const T& w() const noexcept { return q_.w; }

    // ------------------------------- algebra --------------------------------

    // Exact equality (component-wise)
    friend constexpr bool operator==(const quat_t& a, const quat_t& b) noexcept {
        return a.q_.x == b.q_.x && a.q_.y == b.q_.y && a.q_.z == b.q_.z && a.q_.w == b.q_.w;
    }
    friend constexpr bool operator!=(const quat_t& a, const quat_t& b) noexcept { return !(a==b); }

    // Length / normalization
    T length2() const noexcept { return q_.x*q_.x + q_.y*q_.y + q_.z*q_.z + q_.w*q_.w; }
    T length()  const noexcept { using std::sqrt; return sqrt(length2()); }

    quat_t normalized() const noexcept {
        backend_t n = glm::normalize(q_);
        return quat_t(n);
    }

    // Conjugate / inverse (for unit quaternions, inverse == conjugate)
    inline quat_t conjugate() const noexcept { return quat_t(glm::conjugate(q_)); }

    inline quat_t inverse() const noexcept {
        // glm::inverse handles non-unit quats; for unit, it equals conjugate
        return quat_t(glm::inverse(q_));
    }

    inline mat_t<4,4,T> toMat4() const noexcept {
        return mat_t<4,4,T>(glm::toMat4(q_));
    }

    // Dot
    inline friend T dot(const quat_t& a, const quat_t& b) noexcept {
        return a.q_.x*b.q_.x + a.q_.y*b.q_.y + a.q_.z*b.q_.z + a.q_.w*b.q_.w;
    }

    // Multiplication (composition). GLM order: q1*q2 applies q2, then q1.
    inline friend quat_t operator*(const quat_t& a, const quat_t& b) noexcept {
        return quat_t(a.q_ * b.q_);
    }

    inline quat_t& operator*=(const quat_t& r) noexcept {
        q_ = q_ * r.q_;
        return *this;
    }

    inline friend vec_t<3,T> operator*(const quat_t<T>& q, const vec_t<3, T>& v ) noexcept {
        glm::vec<3,T,glm::defaultp> gv(v[0], v[1], v[2]);
        glm::vec<3,T,glm::defaultp> gr = q.q_ * gv; // GLM overload does the sandwich internally
        return vec_t<3,T>(gr.x, gr.y, gr.z);
    }

    inline vec_t<3,T> rotate(const vec_t<3,T>& v) const noexcept {
        return (*this) * v; // use operator*
    }


    // Spherical linear interpolation
    friend quat_t slerp(const quat_t& a, const quat_t& b, T t) noexcept {
        return quat_t(glm::slerp(a.q_, b.q_, t));
    }
};

// ----------------------- approx equality helper (quat_t) ----------------------

template<typename T>
inline bool equal(const quat_t<T>& a, const quat_t<T>& b, T threshold = math::epsilon<T>()) noexcept {
    for (int i=0;i<4;++i) {
        if (std::abs(a[i] - b[i]) > threshold) return false;
    }
    return true;
}

template<typename T>
inline bool approxEqual(const quat_t<T>& a, const quat_t<T>& b,
                         T abs_eps = mgl::math::abs_eps, T rel_eps = mgl::math::rel_eps) noexcept
{
    const T ax=a.x(), ay=a.y(), az=a.z(), aw=a.w();
    const T bx=b.x(), by=b.y(), bz=b.z(), bw=b.w();

    auto within = [&](T x, T y) {
        const T diff = std::abs(x - y);
        const T tol  = std::max(abs_eps, rel_eps * std::max(std::abs(x), std::abs(y)));
        return diff <= tol;
    };
    return within(ax,bx) && within(ay,by) && within(az,bz) && within(aw,bw);
}

using quat = quat_t<float>;
using quatd = quat_t<double>;

} // namespace mgl::math
