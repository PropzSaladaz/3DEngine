#include <math/mathVector.hpp>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

TEST(MathVectorTest, VectorAddition)
{
    // if
    math::vec vec(2.0f, 3.0f, 1.0f);
    math::vec vec2(3.0f, 4.0f, 1.0f);
    // when
    math::vec sum = vec + vec2;
    std::cout << sum << std::endl;
    // then 
    math::vec result(5.0f, 7.0f, 2.0f);
    EXPECT_EQ(sum, result);
}

TEST(MathVectorTest, DotProduct)
{
    // if
    math::vec u(1.0f, 0.0f, 0.0f);
    math::vec v(0.5f, 0.5f, 0.0f);
    // when
    GLfloat dotProd = u.dot(v);
    GLfloat dotProd2 = u.length() * v.length() * (float)cos(PI / 4);
    // then 
    ASSERT_NEAR(dotProd, dotProd2, 1e-6);
}

TEST(MathVectorTest, CrossProduct)
{
    // if
    math::vec u(1.0f, 0.0f, 0.0f);
    math::vec v(0.5f, 0.5f, 0.0f);
    math::vec w(0.0f, 1.0f, 0.5f);
    GLfloat c = 5.0f;

    // NO COMMUTATIVITY PROPERTY
    // when
    math::vec crossProd = u.cross(v);
    math::vec invertedCrossProd = v.cross(u);
    // then
    ASSERT_EQ(crossProd, -invertedCrossProd);

    // FORMULA EQUIVALENCE
    // when
    GLfloat crossProdNorm = u.cross(v).length();
    GLfloat crossProdNorm2 = u.length() * v.length() * (float)sin(PI / 4);
    // then 
    ASSERT_NEAR(crossProdNorm, crossProdNorm2, 1e-6);

    // DISTRIBUTIVE PROPERTY
    // when
    math::vec distributive = u.cross(v + w);
    math::vec distributive2 = u.cross(v) + u.cross(w);
    // then
    ASSERT_EQ(distributive, distributive2);

    // ASSOCIATIVE PROPERTY
    //when
    math::vec assoc = u.cross(c * v);
    math::vec assoc2 = c * u.cross(v);
    math::vec assoc3 = (c * u).cross(v);
    //then
    ASSERT_EQ(assoc, assoc2);
    ASSERT_EQ(assoc, assoc3);

}