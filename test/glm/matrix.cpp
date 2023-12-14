#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp> 

TEST(MatrixTest, Multiplication)
{
    // if
    glm::mat2x3 A(1.0f, 5.0f, 0.0f,
                     3.0f, 2.0f, 4.0f);

    glm::mat4x2 B(3.0f, 2.0f,
                     6.0f, 7.0f,
                     9.0f, 8.0f,
                     4.0f, 3.0);

    glm::mat4x3 res(9.0f, 19.0f, 8.0f,
                  27.0f, 44.0f, 28.0f,
                  33.0f, 61.0f, 32.0f,
                  13.0f, 26.0f, 12.0f);

    // when
    glm::mat4x3 mult = A * B;
    // then
    EXPECT_EQ(mult, res);
}

TEST(MatrixTest, Properties)
{
    // if
    glm::mat2 A(1.0f,  0.0f,
                3.0f, 4.0f);
    glm::mat2 B(3.0f, 2.0f,
                6.0f, 7.0f);
    glm::mat2 C(-1.0f, 5.0f,
                 0.0f, 3.0f);

    // NO COMMUTATIVITY
    // when
    glm::mat2 mult     = A * B;
    glm::mat2 commMult = B * A;
    // then
    EXPECT_NE(mult, commMult);

    // ASSOCIATIVITY
    // when
    glm::mat2 add  = (A+B)+C;
    glm::mat2 add2 = A+(B+C);
    glm::mat2 mul  = (A*B)*C;
    glm::mat2 mul2 = A*(B*C);
    // then
    EXPECT_EQ(add, add2);
    EXPECT_EQ(mul, mul2);

    // DISTRIBUTIVE
    // when
    glm::mat2 dist  = A*(B+C);
    glm::mat2 dist2 = A*B + A*C;
    // then
    EXPECT_EQ(dist, dist2);
}

TEST(MatrixTest, Transpose)
{
    // if
    glm::mat2x3 A(1.0f,  3.0f, 5.0f,
                  2.0f, 4.0f, 6.0f);
    glm::mat3x2 B(2.0f,  1.0f,
                  -1.0f, 0.0f,
                  3.0f, 2.0f);

    glm::mat3x2 A_T(1.0f, 2.0f,
                    3.0f, 4.0f,
                    5.0f, 6.0f);
    // when
    glm::mat3x2 A_t = glm::transpose(A);
    glm::mat3x3 AB_T = glm::transpose(A*B);
    glm::mat3x3 AB_T2 = glm::transpose(B) * glm::transpose(A);
    // then
    EXPECT_EQ(A_T, A_t);
    EXPECT_EQ(AB_T, AB_T2);
}