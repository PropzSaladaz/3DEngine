#include <gtest/gtest.h>
#include <cmath>
#include <math/quaternion.hpp>
#include <math/vector.hpp>
#include "math/transform/quaternion.hpp"

using namespace mgl::math;

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

TEST(QuatTest, DefaultIsIdentity)
{
    quat q;
    EXPECT_FLOAT_EQ(q.w(), 1.0f);
    EXPECT_FLOAT_EQ(q.x(), 0.0f);
    EXPECT_FLOAT_EQ(q.y(), 0.0f);
    EXPECT_FLOAT_EQ(q.z(), 0.0f);
}

TEST(QuatTest, RawConstructorXYZW)
{
    quat q(2.0f, 3.0f, 4.0f, 1.0f);
    EXPECT_FLOAT_EQ(q.w(), 1.0f);
    EXPECT_FLOAT_EQ(q.x(), 2.0f);
    EXPECT_FLOAT_EQ(q.y(), 3.0f);
    EXPECT_FLOAT_EQ(q.z(), 4.0f);
}

TEST(QuatTest, IdentityNamedConstructor)
{
    auto q = quat::identity();
    EXPECT_FLOAT_EQ(q.w(), 1.0f);
    EXPECT_FLOAT_EQ(q.x(), 0.0f);
    EXPECT_FLOAT_EQ(q.y(), 0.0f);
    EXPECT_FLOAT_EQ(q.z(), 0.0f);
}

// -----------------------------------------------------------------------------
// Basic Algebra
// -----------------------------------------------------------------------------

TEST(QuatTest, ConjugateOfUnitQuatNegatesVectorPart)
{
    quat q(2.0f, -3.0f, 4.0f, 1.0f);;
    auto qc = q.conjugate();

    EXPECT_FLOAT_EQ(qc.x(), -2.0f);
    EXPECT_FLOAT_EQ(qc.y(),  3.0f);
    EXPECT_FLOAT_EQ(qc.z(), -4.0f);
    EXPECT_FLOAT_EQ(qc.w(),  1.0f);
}

TEST(QuatTest, NormalizeKeepsDirection)
{
    quat q(0.0f, 0.0f, 0.0f, 2.0f);  // magnitude 2
    auto n = q.normalized();

    EXPECT_NEAR(n.w(), 1.0f, mgl::math::abs_eps);
    EXPECT_NEAR(n.x(), 0.0f, mgl::math::abs_eps);
    EXPECT_NEAR(n.y(), 0.0f, mgl::math::abs_eps);
    EXPECT_NEAR(n.z(), 0.0f, mgl::math::abs_eps);
}

TEST(QuatTest, InverseOfUnitQuatIsConjugate)
{
    quat q = quat::fromY(1.0f); // any unit quat
    auto inv = q.inverse();
    auto conj = q.conjugate();
    EXPECT_TRUE(approxEqual(inv, conj));
}

TEST(QuatTest, UnitNormPreserved_Composition) {
    quat qx = quat::fromX(static_cast<float>(M_PI * 0.3f));
    quat qy = quat::fromY(static_cast<float>(M_PI * 0.5f));
    quat q  = qy * qx;
    // inverse == conjugate implies unit-length
    EXPECT_TRUE(approxEqual(q.inverse(), q.conjugate()));
}

TEST(QuatTest, CompositionEqualsSequential_YthenX) {
    quat qY = quat::fromY(static_cast<float>(M_PI_2)); // 90º Y
    quat qX = quat::fromX(static_cast<float>(M_PI_2)); // 90º X
    vec3 p{0, 0, 1};

    // qX * qY * p == (qX * qY) * p
    vec3 comp = (qX * qY) * p;
    vec3 seq = qX * (qY * p);

    EXPECT_TRUE(approxEqual(comp, seq));

    EXPECT_TRUE(approxEqual(seq, vec3{1, 0, 0}));
}

TEST(QuatTes, MatrixConversionMatchesQuaternionRotation) {
    const vec3 p{1, 0, 0};
    quat qY = quat::fromY(static_cast<float>(M_PI_2));

    vec3 pq = qY * p;
    // matrix route
    auto R = qY.toMat4();                 // mat<4,4,float>
    mgl::math::vec_t<4,float> ph{p[0], p[1], p[2], 1.0f};
    auto ph2 = R * ph;
    vec3 pm{ph2[0], ph2[1], ph2[2]};

    vec3 expected{0,0,-1};
    EXPECT_TRUE(approxEqual(pq, expected));
    EXPECT_TRUE(approxEqual(pm, expected));
}

// -----------------------------------------------------------------------------
// Rotation
// -----------------------------------------------------------------------------

TEST(QuatTest, RotateAroundY90Degrees)
{
    vec3 v(5.0f, 0.0f, 0.0f);
    auto q = quat::fromY(static_cast<float>(M_PI_2)); // +90° around Y

    auto vr = mgl::math::rotate(q, v);

    // Expected: (5,0,0) rotated 90° around +Y = (0,0,-5)
    EXPECT_TRUE(approxEqual(vr, vec3(0.0f, 0.0f, -5.0f)));
}

TEST(QuatTest, RotateIdentityDoesNothing)
{
    auto q = quat::identity();
    vec3 v(1.5f, -2.0f, 7.0f);
    auto vr = mgl::math::rotate(q, v);
    EXPECT_TRUE(approxEqual(vr, v));
}

TEST(QuatTest, CompositionOrderGLMStyle)
{
    // y 90°, then x 90°
    auto qy = quat::fromY(static_cast<float>(M_PI_2));
    auto qx = quat::fromX(static_cast<float>(M_PI_2));

    // Composition: q = q2 * q1 means apply q1 then q2
    auto q = qx * qy;

    vec3 v(0.0f, 0.0f, 1.0f);
    auto vr = mgl::math::rotate(q, v);

    // Expected: rotate (0,0,1) by y90 → (1,0,0); then x90 → (1,-0,0)
    EXPECT_TRUE(approxEqual(vr, vec3(1.0f, 0.0f, 0.0f)));
}

// -----------------------------------------------------------------------------
// From Rotation Between
// -----------------------------------------------------------------------------

// Helper: deterministic unitize
static vec3 unitize(vec3 v) {
    float n2 = v.length2();
    if (n2 == 0.0f) return vec3{1,0,0};
    return v / std::sqrt(n2);
}

TEST(QuatTest, FromRotationBetweenAlignedVectorsIsIdentity)
{
    vec3 a(0.0f, 1.0f, 0.0f);
    vec3 b(0.0f, 1.0f, 0.0f);

    auto q = mgl::math::rotationUnit(a, b);
    EXPECT_TRUE(approxEqual(q, quat::identity()));
}

TEST(QuatTest, FromRotationBetweenOppositeVectors)
{
    vec3 a(0.0f, 1.0f, 0.0f);
    vec3 b(0.0f, -1.0f, 0.0f);

    quat q = mgl::math::rotationUnit(a, b);
    auto vr = mgl::math::rotate(q, a);

    EXPECT_TRUE(approxEqual(vr, b));
}

// 1) Identity: a == b
TEST(QuatTest, Between_Identity_NoRotation) {
    vec3 a = unitize(vec3{0.3f,-0.7f,0.6f});
    vec3 b = a;
    quat q = mgl::math::rotationUnit(a, b);
    EXPECT_TRUE(approxEqual(q.inverse(), q.conjugate())); // unit
    EXPECT_TRUE(approxEqual(q * a, b));       // maps a->b
    EXPECT_TRUE(approxEqual(q, quat::identity()));        // near identity
}

// 2) Opposite: a == -b => 180° about axis ⟂ a
TEST(QuatTest, Between_Opposite_180deg) {
    vec3 a = unitize(vec3{0.3f,0.5f,-0.8f});
    vec3 b = a * -1.0f;

    quat q = mgl::math::rotationUnit(a, b);
    EXPECT_TRUE(approxEqual(q.inverse(), q.conjugate()));
    EXPECT_TRUE(approxEqual(q * a, b, 5e-5f, 5e-5f));

    // Roundtrip via inverse
    EXPECT_TRUE(approxEqual(q.inverse() * b, a, 5e-5f, 5e-5f));
}

// 3) Orthogonal 90° cases on principal axes
TEST(QuatTest, Between_Orthogonal_PrincipalAxes) {
    // X -> Z
    {
        vec3 a{1,0,0}, b{0,0,1};
        quat q = mgl::math::rotationUnit(a, b);
        EXPECT_TRUE(approxEqual(q * a, b));
    }
    // Z -> -X
    {
        vec3 a{0,0,1}, b{-1,0,0};
        quat q = mgl::math::rotationUnit(a, b);
        EXPECT_TRUE(approxEqual(q * a, b));
    }
    // Y -> -Z
    {
        vec3 a{0,1,0}, b{0,0,-1};
        quat q = mgl::math::rotationUnit(a, b);
        EXPECT_TRUE(approxEqual(q * a, b));
    }
}

// 4) Random pairs (stability)
TEST(QuatTest, Between_RandomPairs_MapExactlyAndAreUnit) {
    const vec3 s[] = {
        unitize(vec3{ 0.1f,  0.2f,  0.3f}),
        unitize(vec3{-0.9f,  0.4f, -0.1f}),
        unitize(vec3{ 0.0f,  1.0f,  0.0f}),
        unitize(vec3{ 0.7f, -0.6f,  0.2f}),
        unitize(vec3{-0.2f, -0.3f,  0.9f}),
    };
    for (auto& a : s) {
        for (auto& b : s) {
            quat q = mgl::math::rotationUnit(a, b);
            EXPECT_TRUE(approxEqual(q.inverse(), q.conjugate()));
            EXPECT_TRUE(approxEqual(q * a, b, 5e-5f, 5e-5f));
            EXPECT_TRUE(approxEqual(q.inverse() * b, a, 5e-5f, 5e-5f));
        }
    }
}

// 5) Near-parallel / near-antiparallel edges
TEST(QuatTest, Between_EdgeNearParallelAndNearAntiparallel) {
    vec3 a{1,0,0};

    // Near-parallel (+ε in Y)
    {
        vec3 b = unitize(vec3{1.0f, 1e-6f, 0.0f});
        quat q = mgl::math::rotationUnit(a, b);
        EXPECT_TRUE(approxEqual(q * a, b, 1e-5f, 1e-5f));
        EXPECT_TRUE(approxEqual(q.inverse(), q.conjugate()));
    }

    // Near-antiparallel (-X + small Y)
    {
        vec3 b = unitize(vec3{-1.0f, 1e-4f, 0.0f});
        quat q = mgl::math::rotationUnit(a, b);
        EXPECT_TRUE(approxEqual(q * a, b, 5e-5f, 5e-5f));
        EXPECT_TRUE(approxEqual(q.inverse(), q.conjugate()));
    }
}

// 6) Consistency with axis-angle sign ambiguity (q and -q same rotation)
TEST(QuatTest, Between_SignAmbiguityNoEffect) {
    vec3 a = unitize(vec3{0.2f, -0.4f, 0.9f});
    vec3 b = unitize(vec3{-0.6f, 0.1f, 0.8f});
    quat q  = mgl::math::rotationUnit(a, b);
    quat qm = quat{-q.x(), -q.y(), -q.z(), -q.w()}; // if you expose accessors

    vec3 r1 = q  * a;
    vec3 r2 = qm * a;
    EXPECT_TRUE(approxEqual(r1, r2));
    EXPECT_TRUE(approxEqual(r1, b));
}

// -----------------------------------------------------------------------------
// Slerp
// -----------------------------------------------------------------------------

TEST(QuatTest, SlerpEndsPointsMatch)
{
    auto a = quat::fromY(0.0f);
    auto b = quat::fromY(static_cast<float>(M_PI_2)); // 90°

    EXPECT_TRUE(approxEqual(slerp(a, b, 0.0f), a));
    EXPECT_TRUE(approxEqual(slerp(a, b, 1.0f), b));
}

TEST(QuatTest, SlerpMidpointIsNormalized)
{
    auto a = quat::fromY(0.0f);
    auto b = quat::fromY(static_cast<float>(M_PI_2));

    auto m = slerp(a, b, 0.5f);
    auto mn = m.normalized();

    EXPECT_TRUE(approxEqual(m, mn)); // midpoint should already be unit
}
