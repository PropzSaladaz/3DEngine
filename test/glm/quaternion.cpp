#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp> 
#include <glm/gtx/quaternion.hpp>


TEST(QuaternionTest, Rotation)
{
    glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
    glm::quat q = glm::angleAxis(glm::radians(90.0f), axis);
    glm::quat qi = { 0.0f, 5.0f, 0.0f, 0.0f };
    glm::quat qe = { 0.0f, 0.0f, 0.0f, -5.0f };
    glm::quat qc = glm::conjugate(q);
    glm::quat qf2 = q * qi * qc;
    ASSERT_TRUE(glm::all(glm::equal(qf2, qe, static_cast<float>(1.0e-5))));

    q = glm::angleAxis(glm::radians(1.0f), axis);
}



TEST(QuaternionTest, RotationComposition)
{
    glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
    glm::vec3 point = { 1.0f, 0.0f, 0.0f };
    glm::quat qi(0.0f, point);
    
    // rotate 90 degrees in YY
    glm::quat q = glm::angleAxis(glm::radians(90.0f), axis);
    glm::quat qc = glm::conjugate(q);
    glm::quat qf1 = q * qi * qc;

    // rotate 90 degrees in XX
    glm::quat q2 = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat qc2 = glm::conjugate(q2);
    glm::quat qf2 = q2 * qf1 * qc2;

    // composition
    glm::quat qcomp = q2 * q;
    glm::quat qc3 = glm::conjugate(qcomp);
    glm::quat qf3 = qcomp * qi * qc3;

    glm::quat qe = { 0.0f, 0.0f, 1.0f, 0.0f };
    ASSERT_TRUE(glm::all(glm::equal(qf2, qe, static_cast<float>(1.0e-5))));
    ASSERT_TRUE(glm::all(glm::equal(qf3, qe, static_cast<float>(1.0e-5))));
}

TEST(QuaternionTest, MatrixConversion)
{
    glm::vec3 point = { 1.0f, 0.0f, 0.0f };
    glm::quat qi(0.0f, point);

    // rotate 90 degrees in YY
    glm::quat q = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat qc = glm::conjugate(q);
    glm::quat qf1 = q * qi * qc;
    glm::mat4 t = glm::toMat4(q);
    glm::vec3 pf = t * glm::vec4(point, 1.0f);

    glm::quat qe = { 0.0f, 0.0f, 0.0f, -1.0f };
    glm::vec3 pe = { 0.0f, 0.0f, -1.0f };
    ASSERT_TRUE(glm::all(glm::equal(qf1, qe, static_cast<float>(1.0e-5))));
    ASSERT_TRUE(glm::all(glm::equal(pf, pe, static_cast<float>(1.0e-5))));
}

TEST(QuaternionTest, Slerp)
{
    const glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
    glm::quat q0 = glm::angleAxis(glm::radians(0.0f), axis);
    glm::vec3 pi = { 0, 0, 1 };
    std::cout << "      q0=" << glm::to_string(q0) << std::endl;
    // force clockwise rotation
    glm::quat q1 = glm::angleAxis(glm::radians(179.999f), axis);
    glm::vec3 p1e = { 0, 0, -1 };
    std::cout << "      q1=" << glm::to_string(q1) << std::endl; 
    glm::quat qe = glm::angleAxis(glm::radians(90.0f), axis);
    glm::vec3 pee = { 1, 0, 0 };
    std::cout << "      qe=" << glm::to_string(qe) << std::endl; 

    glm::quat qSlerp0 = glm::slerp(q0, q1, 0.0f); 
    std::cout << " qSlerp0=" << glm::to_string(qSlerp0) << std::endl; 
    ASSERT_TRUE(glm::all(glm::equal(qSlerp0, q0, static_cast<float>(1.0e-4))));

    glm::quat qSlerp1 = glm::slerp(q0, q1, 1.0f); 
    glm::quat qs1 = qSlerp1 * glm::quat(0, pi) * glm::conjugate(qSlerp1);
    glm::vec3 p1c = glm::vec3(qs1.x, qs1.y, qs1.z);
    std::cout << " p1c=" << glm::to_string(p1c) << std::endl;
    ASSERT_TRUE(glm::all(glm::equal(p1c, p1e, static_cast<float>(1.0e-4))));

    // note that quaternions can represent same rotation with different values
    // thus the following assert will not validate correctly. Validate final point instead
    //ASSERT_TRUE(glm::all(glm::equal(qSlerp1, q1, static_cast<float>(1.0e-5))));

    glm::quat qSlerp = glm::slerp(q0, q1, 0.5f);
    glm::quat qs = qSlerp * glm::quat(0, pi) * glm::conjugate(qSlerp);
    glm::vec3 pec = glm::vec3(qs.x, qs.y, qs.z);
    std::cout << " pec=" << glm::to_string(pec) << std::endl;
    ASSERT_TRUE(glm::all(glm::equal(pec, pee, static_cast<float>(1.0e-4))));
    std::cout << "  qSlerp=" << glm::to_string(qSlerp) << std::endl; 
    ASSERT_TRUE(glm::all(glm::equal(qSlerp, qe, static_cast<float>(1.0e-4))));
}