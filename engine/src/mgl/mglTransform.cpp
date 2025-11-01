#include <mgl/mglTransform.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/mglSimulation.hpp>
#include <utils/Logger.hpp>
#include "math/math.hpp"

namespace mgl {

// deal with length 0 vectors & make it order-dependent
f32 angle(const math::vec2& vec, const math::vec2& vec2) {
    if (vec.length() < math::epsilon<float>() || 
        vec2.length() < math::epsilon<float>()) {
        return 0;
    }
    math::vec3 cross = math::cross(math::vec3(vec, 0.0f), math::vec3(vec2, 0.0f));
    return cross.z() > 0 ? math::angle(vec, vec2) : -math::angle(vec, vec2);
}

f32 angle(const math::vec3& vec, const math::vec3& vec2) {
    if (vec.length() < math::epsilon<float>() ||
        vec2.length() < math::epsilon<float>()) {
        return 0;
    }
    return math::angle(vec, vec2);
}

////////////////////////////////////////////////////////////////// Transform
Transform::Transform() : ITimeUpdateable() {
    setPosition(0, 0, 0);
}

Transform::Transform(const math::vec3& _up, const  math::vec3& _right, 
                                const  math::vec3& _front) : Transform() {
    up = _up;
    right = _right;
    front = _front;
    targetPoint = positionV + front; // look at front
}

Transform::Transform(f32 x, f32 y, f32 z) 
    : Transform(math::vec3(x, y, z)) {}

Transform::Transform(const math::vec3 &center) : Transform() {
    targetPoint = positionV + front; // look at front
    setPosition(center);
}

Transform::Transform(const Transform* t) : Transform() {
    setTransform(t);
}

Transform::~Transform() {}

void Transform::resetTransform() {
    transformMatrix = math::mat4::identity();
    positionV = ORIGIN;
    up    = YY;
    right = -XX;
    front = ZZ;
    math::quat rotation{ 0.0f , 0.0f, 0.0f, 0.0f };
    math::vec3 scaleV{ 1, 1, 1 };
    math::vec3 translationV{ 0, 0, 0 };
}

/**
    Setting transform from another does not set the targetTransform, 
    nor the tracking target state
*/
void Transform::setTransform(const Transform* t) {
    positionV   = t->getPosition();
    up          = t->getUpV();
    right       = t->getRightV();
    front       = t->getFrontV();
    rotation    = t->getRotationQuat();
    scaleV      = t->getScale();
    targetPoint = t->getTargetPoint();
    computeTransformMatrix();
}

////////////////////////////////////////////////////////////////// Getters
const f32* Transform::getTransformMatrixPtr() const {
    return transformMatrix.data();
}

const math::mat4 Transform::getTransformMatrix() const {
    return transformMatrix;
}

const math::vec3 Transform::getPosition() const {
    return positionV;
}
const math::vec3 Transform::getUpV() const {
    return up;
}
const math::vec3 Transform::getRightV() const {
    return right;
}
const math::vec3 Transform::getFrontV() const {
    return front;
}

const math::vec3 Transform::getTargetPoint() const {
    return targetPoint;
}

const math::vec3 Transform::getScale() const {
    return scaleV;
}

const math::quat Transform::getRotationQuat() const {
    return rotation;
}

//////////////////////////////////////////////////////////////// Track Target

Transform* Transform::lookAt(const Transform* target) {
    math::vec3 newFront = math::normalize(target->getPosition() - positionV);
    math::quat rot = math::rotationUnit(front, newFront);

    // check if we are heads down or not
    math::vec3 desiredUp = YY;

    right = math::cross(newFront, desiredUp);
    up = math::cross(right, newFront);

    front = newFront;
    targetPoint = target->getPosition();

    math::vec3 newUp = rot * up;
    math::quat rot2 = math::rotationUnit(newUp, desiredUp);

    rotation = rot2 * rot * rotation;
    computeTransformMatrix();
    return this;
}


Transform* Transform::lookAtFrom(const Transform* target, const Transform* source) {
    setPosition(source->getPosition());
    return lookAt(target);
}


//////////////////////////////////////////////////////////////////////////// Private/Protected

void Transform::computeTransformMatrix() {
    math::mat4 scaleM     = math::scale(I, scaleV);
    math::mat4 translateM =  math::translate(I, positionV);
    math::mat4 rotationM  = rotation.toMat4();
    transformMatrix = translateM * rotationM * scaleM;
}

void Transform::normalizeTransform() {
    up    = math::normalize(up);
    right = math::normalize(right);
    front = math::normalize(front);
}

//////////////////////////////////////////////////////////////////////////// Rotation
Transform* Transform::rotate2D(f32 angleDegree) {
    return rotate(angleDegree, ZZ);
}

Transform* Transform::rotate(f32 angleDegrees, math::vec3 rotationAxis) {
    // needed to force CC rotation, else it represents the rotation
    // with a clockwise rot quaternion
    if (angleDegrees == 180.0f) angleDegrees -= FLOAT_THREASHOLD;
    return rotateRad(math::radians(angleDegrees), rotationAxis);
}

Transform* Transform::setRotationQuat(const math::quat& rot) {
    rotation = rot;
    math::quat newUp = rot * math::quat(YY, 0.0f) * rot.conjugate();
    math::quat newRight = rot * math::quat(-XX, 0.0f) * rot.conjugate();
    math::quat newFront = rot * math::quat(ZZ, 0.0f) * rot.conjugate();
    // TODO store initial XX, YY, ZZ - if camera is initialized with -ZZ, then on new setrotation, the ZZ will not be good
    computeTransformMatrix();
    return this;
}

// used internally for efficiency
Transform* Transform::rotateRad(f32 angleRads, math::vec3 rotationAxis) {
    if (angleRads == 0) return this;
    
    math::quat rot = math::quat::fromAxisAngle(rotationAxis, angleRads);

    // update directional vectors
    math::quat newUp = rot * math::quat(up, 0.0f) * rot.conjugate();
    math::quat newRight = rot * math::quat(right, 0.0f) * rot.conjugate();
    math::quat newFront = rot * math::quat(front, 0.0f) * rot.conjugate();
    up = math::vec3(newUp.x(), newUp.y(), newUp.z());
    right = math::vec3(newRight.x(), newRight.y(), newRight.z());
    front = math::vec3(newFront.x(), newFront.y(), newFront.z());
    // convert target point to directional vector
    math::quat newTargetPoint = rot * math::quat(targetPoint - positionV, 0.0f) * rot.conjugate();
    // add the directional vector to position to get the target
    targetPoint = math::vec3(newTargetPoint.x(), newTargetPoint.y(), newTargetPoint.z()) + positionV;
    // update rotation quat & transformMatrix
    rotation = rot * rotation;
    computeTransformMatrix();
    return this;
}



//////////////////////////////////////////////////////////////////////////// Scaling
Transform* Transform::scale2D(f32 scale) {
    return scale2D(math::vec2(scale));
}

Transform* Transform::scale2D(math::vec2 scale_v) {
    return scale(math::vec3(scale_v, 1.0f));
}
Transform* Transform::scale(f32 scale_factor) {
    return scale(math::vec3(scale_factor));
}
Transform* Transform::setScale(const math::vec3& scale) {
    scaleV = scale;
    computeTransformMatrix();
    return this;
}
Transform* Transform::scale(const math::vec3 &scale_v) {
    scaleV = scaleV * scale_v;
    computeTransformMatrix();
    return this;
}

//////////////////////////////////////////////////////////////////////////// Translating

Transform* Transform::setPosition(f32 x, f32 y, f32 z) {
    return setPosition(math::vec3(x, y, z));
}
Transform* Transform::setPosition(const math::vec3& newPos) {
    return translate(newPos - positionV);
}

Transform* Transform::translate2D(f32 x, f32 y){
    return translate(math::vec3(x, y, 0.0f));
}

Transform* Transform::translate(f32 x, f32 y, f32 z) {
    return translate(math::vec3(x, y, z));
}

Transform* Transform::translate(const math::vec3& translateVec) {
    positionV += translateVec;
    // set new translation in transform matrix directly
    // since translation is the last transform
    transformMatrix(0,3) = positionV.x();
    transformMatrix(1,3) = positionV.y();
    transformMatrix(2,3) = positionV.z();
    if (!trackingEnabled) targetPoint += translateVec;
    return this;
}

/////////////////////////////////////////////////////////////////// TimeUpdateable

void Transform::update(f32 deltaTime) {
    lookAt(targetTransform);
}

void Transform::trackTarget(const Transform* target) {
    if (target->getPosition() == positionV) {
        MGL_WARN("TRANSFORM::TRACK_TARGET::WARNING");
        MGL_WARN("Tracking transforms with the exact same position"
            "as the tracker results in undefined behavior");
        exit(EXIT_FAILURE);
    }
    targetTransform = target;
    trackingEnabled = true;
    Simulation::getInstance().trackUpdateable(getId(), this);
}
void Transform::disableTargetTracking() {
    targetTransform = nullptr;
    trackingEnabled = false;
    Simulation::getInstance().stopTracking(getId());
}

}