#include <mgl/camera/mglCamera.hpp>
#include <mgl/mglInputManager.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
namespace mgl {

	//////////////////////////////////////////////////////////////////// Camera
	Camera::Camera(GLuint _bindingPoint) : Transform(YY, XX, -ZZ),
		bindingPoint(_bindingPoint) {
		glGenBuffers(1, &uboId);
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, 0, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// register resize callback
		InputManager::getInstance().registerWindowSizeCallback(
			[this](GLuint width, GLuint height) {
			updateScreenRatio(width, height);
			});
	}

	void Camera::orthoResize(OrthoParams* params,
		GLuint winx, GLuint winy) {
		GLfloat screenRation = static_cast<float>(winx) / winy;
		GLfloat verticalExtent = (params->top - params->bottom) / 2.0f;
		GLfloat horizontalExtent = screenRation * verticalExtent;
		params->left = -horizontalExtent;
		params->right = horizontalExtent;
	}
	void Camera::perspectiveResize(PerspectiveParams* params,
		GLuint winx, GLuint winy) {
		GLfloat screenRation = static_cast<float>(winx) / winy;
		params->screenRatio = screenRation;
	}

	void Camera::setActive() {
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboId);
		setViewMatrix();
		setProjectionMatrix();
	}

	void Camera::pitch(GLfloat degrees) {
		rotate(degrees, getRightV());
		updateViewMatrix();
	}

	void Camera::yaw(GLfloat degrees) {
		rotate(degrees, mgl::YY);
		updateViewMatrix();
	}

	glm::vec3 Camera::getFrontV() const {
		return Transform::getFrontV();
	}
	glm::vec3 Camera::getRightV() const {
		return Transform::getRightV();
	}
	glm::vec3 Camera::getUpV() const {
		return Transform::getUpV();
	}

	glm::vec3 Camera::getPosition() const {
		return Transform::getPosition();
	}

	glm::mat4 Camera::getViewMatrix() const {
		return viewMatrix;
	}
	/////////////////////////////////////////////////////////////////// Position

	void Camera::lookAtFrom(const Transform* target, const glm::vec3& _position) {
		Transform::setPosition(_position);
		lookAt(target);
	}

	void Camera::lookAtFrom(const Transform* target, const Transform* _position) {
		Transform::setPosition(_position->getPosition());
		lookAt(target);
	}

	void Camera::lookAt(const Transform* target) {
		Transform::lookAt(target); // update up, front, right vectors
		updateViewMatrix();
	}


	void Camera::setPosition(GLfloat x, GLfloat y, GLfloat z) {
		setPosition(glm::vec3(x, y, z));
	}

	void Camera::setPosition(const glm::vec3& newPos) {
		Transform::setPosition(newPos); // update up, front, right vectors
		updateViewMatrix();
	}

	void Camera::translate(GLfloat x, GLfloat y, GLfloat z) {
		translate(glm::vec3(x, y, z));
	}
	void Camera::translate(const glm::vec3& translateDir) {
		Transform::translate(translateDir);
		updateViewMatrix();
	}

	///////////////////////////////////////////////////////////////// OpenGL Shaders

	void Camera::updateViewMatrix() {
		computeViewMatrix();
		setViewMatrix();
	}

	void Camera::computeViewMatrix() {
		viewMatrix = glm::lookAt(positionV, targetPoint, up);
	}

	void Camera::setViewMatrix(const glm::mat4& mat) {
		viewMatrix = mat;
		setViewMatrix();
	}

	void Camera::setViewMatrix() {
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
			glm::value_ptr(viewMatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Camera::updateProjectionMatrix() {
		computeProjectionMatrix();
		setProjectionMatrix();
	}

	void Camera::setProjectionMatrix() {
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
			glm::value_ptr(projectionMatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}