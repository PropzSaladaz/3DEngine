#include <mgl/mglTransform.hpp>
#include <mgl/mglAnimation.hpp>
#include <math/math.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

constexpr float THRESHOLD = static_cast<float>(1.0e-5);

TEST(AnimationTest, InterpolatePosition) {
    auto origin = std::make_unique<mgl::Transform>();
    auto target = std::make_unique<mgl::Transform>();
    target->translate(1.0f, 0.0f, 0.0f);

    mgl::Animation anim(origin.get(), target.get());
   anim.setSpeed(1.0f);
   anim.step(0.5f);

    const glm::vec3 pos    = origin->getPosition();
    const glm::vec3 posExp = {0.5f, 0.0f, 0.0f};
    EXPECT_NEAR(pos.x, posExp.x, THRESHOLD);
    EXPECT_NEAR(pos.y, posExp.y, THRESHOLD);
    EXPECT_NEAR(pos.z, posExp.z, THRESHOLD);
}

TEST(AnimationTest, InterpolateScale) {
    auto origin = std::make_unique<mgl::Transform>();
    auto target = std::make_unique<mgl::Transform>();
    target->scale(glm::vec3(1.0f, 2.0f, 3.0f));

    mgl::Animation anim(origin.get(), target.get());
    anim.setSpeed(1.0f);
    anim.step(0.5f);

    const glm::vec3 scale    = origin->getScale();
    const glm::vec3 scaleExp = {1.0f, 1.5f, 2.0f};
    EXPECT_NEAR(scale.x, scaleExp.x, THRESHOLD);
    EXPECT_NEAR(scale.y, scaleExp.y, THRESHOLD);
    EXPECT_NEAR(scale.z, scaleExp.z, THRESHOLD);
}

TEST(AnimationTest, InterpolateRotation) {
    auto origin = std::make_unique<mgl::Transform>();
    auto target = std::make_unique<mgl::Transform>();
    target->rotate(180.0f, mgl::YY);

    const glm::vec3 initial{0.0f, 0.0f, 1.0f};
    const glm::vec3 expected{1.0f, 0.0f, 0.0f};

    mgl::Animation anim(origin.get(), target.get());
    anim.setSpeed(1.0f);
    anim.step(0.5f);

    const glm::quat rot = origin->getRotationQuat();
    const glm::quat rotated = rot * glm::quat(0.0f, initial) * glm::conjugate(rot);
    const glm::vec3 result{rotated.x, rotated.y, rotated.z};
    EXPECT_NEAR(result.x, expected.x, THRESHOLD);
    EXPECT_NEAR(result.y, expected.y, THRESHOLD);
    EXPECT_NEAR(result.z, expected.z, THRESHOLD);
}

TEST(AnimationTest, InterpolateStep) {
    auto origin = std::make_unique<mgl::Transform>();
    auto target = std::make_unique<mgl::Transform>();
    target->rotate(180.0f, mgl::YY);

    const glm::vec3 initial{0.0f, 0.0f, 1.0f};
    const glm::vec3 expected{0.0f, 0.0f, -1.0f};

    mgl::Animation anim(origin.get(), target.get());
    anim.setSpeed(1.0f);
    for (int i = 0; i < 6; ++i) {
        anim.step(0.2f);
    }

    const glm::quat rot = origin->getRotationQuat();
    const glm::quat rotated = rot * glm::quat(0.0f, initial) * glm::conjugate(rot);
    const glm::vec3 result{rotated.x, rotated.y, rotated.z};
    EXPECT_NEAR(result.x, expected.x, THRESHOLD);
    EXPECT_NEAR(result.y, expected.y, THRESHOLD);
    EXPECT_NEAR(result.z, expected.z, THRESHOLD);
}
