#include <mgl/camera/mglCamera.hpp>
#include <mgl/mglInputManager.hpp>
namespace mgl {

	//////////////////////////////////////////////////////////////////// Camera
	Camera::Camera(i32 _bindingPoint) : Transform(YY, XX, -ZZ),
		bindingPoint(_bindingPoint) {
		glGenBuffers(1, &uboId);
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(math::mat4) * 2 + sizeof(math::vec3), 0, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// register resize callback
		InputManager::getInstance().registerWindowSizeCallback(
			[this](i32 width, i32 height) {
			updateScreenRatio(width, height);
			});
	}

	void Camera::orthoResize(OrthoParams* params,
		i32 winx, i32 winy) {
		GLfloat screenRation = static_cast<float>(winx) / winy;
		GLfloat verticalExtent = (params->top - params->bottom) / 2.0f;
		GLfloat horizontalExtent = screenRation * verticalExtent;
		params->left = -horizontalExtent;
		params->right = horizontalExtent;
	}
	void Camera::perspectiveResize(PerspectiveParams* params,
		i32 winx, i32 winy) {
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

	math::vec3 Camera::getFrontV() const {
		return Transform::getFrontV();
	}
	math::vec3 Camera::getRightV() const {
		return Transform::getRightV();
	}
	math::vec3 Camera::getUpV() const {
		return Transform::getUpV();
	}

	math::vec3 Camera::getPosition() const {
		return Transform::getPosition();
	}

	math::mat4 Camera::getViewMatrix() const {
		return viewMatrix;
	}
	/////////////////////////////////////////////////////////////////// Position

	void Camera::lookAtFrom(const Transform* target, const math::vec3& _position) {
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
		setPosition(math::vec3(x, y, z));
	}

	void Camera::setPosition(const math::vec3& newPos) {
		Transform::setPosition(newPos); // update up, front, right vectors
		updateViewMatrix();
	}

	void Camera::translate(GLfloat x, GLfloat y, GLfloat z) {
		translate(math::vec3(x, y, z));
	}
	void Camera::translate(const math::vec3& translateDir) {
		Transform::translate(translateDir);
		updateViewMatrix();
	}

	///////////////////////////////////////////////////////////////// OpenGL Shaders

	void Camera::updateViewMatrix() {
		computeViewMatrix();
		setViewMatrix();
	}

	void Camera::computeViewMatrix() {
		viewMatrix = math::lookAt(positionV, targetPoint, up);
	}

	void Camera::setViewMatrix(const math::mat4& mat) {
		viewMatrix = mat;
		setViewMatrix();
	}

	void Camera::setViewMatrix() {
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(math::mat4),
			viewMatrix.data());
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(math::mat4), sizeof(math::vec3),
			positionV.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Camera::updateProjectionMatrix() {
		computeProjectionMatrix();
		setProjectionMatrix();
	}

	void Camera::setProjectionMatrix() {
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(math::mat4), sizeof(math::mat4),
			projectionMatrix.data());
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(math::mat4), sizeof(math::vec3),
			positionV.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}