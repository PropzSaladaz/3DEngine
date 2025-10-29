#include <mgl/mglTransform.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/mglSimulation.hpp>
#include <iostream>
#include <utils/Logger.hpp>
#include <string>

namespace mgl {

// deal with length 0 vectors
math::vec3 normalize(const math::vec3& vec) {
    if (math::length(vec) > math::epsilon<float>()) {
        return math::normalize(vec);
    }
    return {0.0f, 0.0f, 0.0f};
}

math::vec2 normalize(const math::vec2& vec) {
    if (math::length(vec) > math::epsilon<float>()) {
        return math::normalize(vec);
    }
    return { 0.0f, 0.0f };
}

// deal with length 0 vectors & make it order-dependent
GLfloat angle(const math::vec2& vec, const math::vec2& vec2) {
    if (math::length(vec) < math::epsilon<float>() || 
        math::length(vec2) < math::epsilon<float>()) {
        return 0;
    }
    math::vec3 cross = math::cross(math::vec3(vec, 0.0f), math::vec3(vec2, 0.0f));
    return cross.z > 0 ? math::angle(vec, vec2) : -math::angle(vec, vec2);
}

GLfloat angle(const math::vec3& vec, const math::vec3& vec2) {
    if (math::length(vec) < math::epsilon<float>() ||
        math::length(vec2) < math::epsilon<float>()) {
        return 0;
    }
    return math::angle(vec, vec2);
}


//  implementation taken from:
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
math::quat rotationBetweenVectors(math::vec3 start, math::vec3 dest) {
    start = normalize(start);
    dest = normalize(dest);

    float cosTheta = math::dot(start, dest);
    math::vec3 rotationAxis;

    if (cosTheta < -1 + 0.001f) {
        // special case when vectors in opposite directions :
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        // This implementation favors a rotation around the Up axis,
        // since it's often what you want to do.
        rotationAxis = math::cross(math::vec3(0.0f, 0.0f, 1.0f), start);
        if (math::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
            rotationAxis = math::cross(math::vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = normalize(rotationAxis);
        return angleAxis(math::radians(180.0f), rotationAxis);
    }

    // Implementation from Stan Melax's Game Programming Gems 1 article
    rotationAxis = math::cross(start, dest);

    float s = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return math::quat(
        s * 0.5f,
        rotationAxis.x() * invs,
        rotationAxis.y() * invs,
        rotationAxis.z() * invs
    );
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

Transform::Transform(GLfloat x, GLfloat y, GLfloat z) 
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
const GLfloat* Transform::getTransformMatrixPtr() const {
    return math::value_ptr(transformMatrix);
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
    math::quat rot = rotationBetweenVectors(front, newFront);

    // check if we are heads down or not
    math::vec3 desiredUp = YY;

    right = math::cross(newFront, desiredUp);
    up = math::cross(right, newFront);

    front = newFront;
    targetPoint = target->getPosition();

    math::vec3 newUp = rot * up;
    math::quat rot2 = rotationBetweenVectors(newUp, desiredUp);

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
    math::mat4 translateM = math::translate(I, positionV);
    math::mat4 rotationM  = math::toMat4(rotation);
    transformMatrix = translateM * rotationM * scaleM;
}

void Transform::normalizeTransform() {
    up    = math::normalize(up);
    right = math::normalize(right);
    front = math::normalize(front);
}

//////////////////////////////////////////////////////////////////////////// Rotation
Transform* Transform::rotate2D(GLfloat angleDegree) {
    return rotate(angleDegree, ZZ);
}

Transform* Transform::rotate(GLfloat angleDegrees, math::vec3 rotationAxis) {
    // needed to force CC rotation, else it represents the rotation
    // with a clockwise rot quaternion
    if (angleDegrees == 180.0f) angleDegrees -= FLOAT_THREASHOLD;
    return rotateRad(math::radians(angleDegrees), rotationAxis);
}

Transform* Transform::setRotationQuat(const math::quat& rot) {
    rotation = rot;
    math::quat newUp = rot * math::quat(0.0f, YY) * math::conjugate(rot);
    math::quat newRight = rot * math::quat(0.0f, -XX) * math::conjugate(rot);
    math::quat newFront = rot * math::quat(0.0f, ZZ) * math::conjugate(rot);
    // TODO store initial XX, YY, ZZ - if camera is initialized with -ZZ, then on new setrotation, the ZZ will not be good
    computeTransformMatrix();
    return this;
}

// used internally for efficiency
Transform* Transform::rotateRad(GLfloat angleRads, math::vec3 rotationAxis) {
    if (angleRads == 0) return this;
    
    math::quat rot = math::angleAxis(angleRads, rotationAxis);

    // update directional vectors
    math::quat newUp = rot * math::quat(0.0f, up) * math::conjugate(rot);
    math::quat newRight = rot * math::quat(0.0f, right) * math::conjugate(rot);
    math::quat newFront = rot * math::quat(0.0f, front) * math::conjugate(rot);
    up = math::vec3(newUp.x, newUp.y, newUp.z);
    right = math::vec3(newRight.x, newRight.y, newRight.z);
    front = math::vec3(newFront.x, newFront.y, newFront.z);
    // convert target point to directional vector
    math::quat newTargetPoint = rot * math::quat(0.0f, targetPoint - positionV) * math::conjugate(rot);
    // add the directional vector to position to get the target
    targetPoint = math::vec3(newTargetPoint.x, newTargetPoint.y, newTargetPoint.z) + positionV;
    // update rotation quat & transformMatrix
    rotation = rot * rotation;
    computeTransformMatrix();
    return this;
}



//////////////////////////////////////////////////////////////////////////// Scaling
Transform* Transform::scale2D(GLfloat scale) {
    return scale2D(math::vec2(scale));
}

Transform* Transform::scale2D(math::vec2 scale_v) {
    return scale(math::vec3(scale_v, 1.0f));
}
Transform* Transform::scale(GLfloat scale_factor) {
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

Transform* Transform::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    return setPosition(math::vec3(x, y, z));
}
Transform* Transform::setPosition(const math::vec3& newPos) {
    return translate(newPos - positionV);
}

Transform* Transform::translate2D(GLfloat x, GLfloat y){
    return translate(math::vec3(x, y, 0.0f));
}

Transform* Transform::translate(GLfloat x, GLfloat y, GLfloat z) {
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

void Transform::update(GLfloat deltaTime) {
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