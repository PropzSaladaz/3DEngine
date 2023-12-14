#include <mgl/mglTransform.hpp>
#include <mgl/mglAnimation.hpp>
#include <glm/glm.hpp>
#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp> 

#define THRESHOLD static_cast<float>(1.0e-5)

TEST(AnimationTest, InterpolatePosition)
{
    // GIVEN origin at (0,0,0), target at (1,0,0)
    mgl::Transform* origin = new mgl::Transform();
    mgl::Transform* target = (new mgl::Transform())
        ->translate(1.0f, 0.0f, 0.0f);
    // WHEN we step 1.0 * 0.5
    mgl::Animation anim(origin, target);
    anim.setSpeed(1.0f);
    anim.step(0.5);
    // THEN
    glm::vec3 pos    = origin->getPosition();
    glm::vec3 posExp = { 0.5f, 0.0f, 0.0f };
    ASSERT_TRUE(glm::all(glm::equal(pos, posExp, THRESHOLD)));
}

TEST(AnimationTest, InterpolateScale)
{
    // GIVEN scale origin is (1, 1, 1) and target (1, 2, 3)
    mgl::Transform* origin = new mgl::Transform();
    mgl::Transform* target = (new mgl::Transform())
        ->scale(glm::vec3(1, 2, 3));
    // WHEN we step 1.0 * 0.5
    mgl::Animation anim(origin, target);
    anim.setSpeed(1.0f);
    anim.step(0.5);
    // THEN exp scale is (1, 1.5, 2)
    glm::vec3 scale = origin->getScale();
    glm::vec3 scaleExp = { 1.0f, 1.5f, 2.0f };
    ASSERT_TRUE(glm::all(glm::equal(scale, scaleExp, THRESHOLD)));
}

TEST(AnimationTest, InterpolateRotation)
{
    // GIVEN rotation origin is 0º, and target is 180º
    mgl::Transform* origin = new mgl::Transform();
    mgl::Transform* target = (new mgl::Transform())
        ->rotate(180.0f, mgl::YY);
    glm::vec3 posi = { 0, 0, 1 };
    glm::vec3 posE = { 1, 0, 0 };
    // WHEN we step 1.0 * 0.5
    mgl::Animation anim(origin, target);
    anim.setSpeed(1.0f);
    anim.step(0.5);
    // THEN exp rotation is 90º
    glm::quat rot = origin->getRotationQuat();
    glm::quat posCQ = rot * glm::quat(0.0f, posi) * glm::conjugate(rot);
    glm::vec3 posC = glm::vec3(posCQ.x, posCQ.y, posCQ.z);
    ASSERT_TRUE(glm::all(glm::equal(posC, posE, THRESHOLD)));
}

TEST(AnimationTest, InterpolateStep)
{
    // GIVEN rotation origin is 0º, and target is 180º
    mgl::Transform* origin = new mgl::Transform();
    mgl::Transform* target = (new mgl::Transform())
        ->rotate(180.0f, mgl::YY);
    glm::vec3 posi = { 0, 0, 1 };
    glm::vec3 posE = { 0, 0, -1 };
    // WHEN we step 1.0 * 0.5
    mgl::Animation anim(origin, target);
    anim.setSpeed(1.0f);
    for (int i = 0; i < 6; i++) { // do 1 step after reaching end of animation
        anim.step(0.2);
    }
    // THEN step should be capped to 1.0, corresponding to end of animation
    glm::quat rot = origin->getRotationQuat();
    glm::quat posCQ = rot * glm::quat(0.0f, posi) * glm::conjugate(rot);
    glm::vec3 posC = glm::vec3(posCQ.x, posCQ.y, posCQ.z);
    ASSERT_TRUE(glm::all(glm::equal(posC, posE, THRESHOLD)));
}

