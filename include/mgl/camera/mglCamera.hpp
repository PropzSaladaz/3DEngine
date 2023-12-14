#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <glm/glm.hpp>
#include <mgl/mglTransform.hpp>

namespace mgl {

	class Camera : Transform {
	public:
		Camera(GLuint bindingPoint);

		static void orthoResize(OrthoParams* params,
			GLuint winx, GLuint winy);
		static void perspectiveResize(PerspectiveParams* params,
			GLuint winx, GLuint winy);

		void lookAtFrom(const Transform* target, const glm::vec3& position);
		void setPerspective(const PerspectiveParams& params);
		void setPerspective(GLfloat fov, GLfloat screen_ratio,
			GLfloat near, GLfloat far);
		void setOrtho(const OrthoParams& params);
		void setOrtho(GLfloat left, GLfloat right, GLfloat bottom,
			GLfloat top, GLfloat near, GLfloat far);

		// overrides from Transform
		Transform* lookAtFrom(const Transform* target, const Transform* _position) override;
		Transform* lookAt(const Transform* target) override;
		Transform* setPosition(GLfloat x, GLfloat y, GLfloat z) override;
		Transform* setPosition(const glm::vec3& newPos) override;
		Transform* translate(GLfloat x, GLfloat y, GLfloat z) override;
		Transform* translate(const glm::vec3& translateDir) override;

		void zoom(GLfloat step);
		void pitch(GLfloat degrees);
		void yaw(GLfloat degrees);
		void setActive();

		void updateScreenRatio(GLuint width, GLuint height);

	private:

		GLuint uboId;
		GLuint bindingPoint;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		void setViewMatrix(const glm::mat4& matrix);
		void setViewMatrix();
		void setProjectionMatrix(const glm::mat4& matrix);
		void setProjectionMatrix();
	};

}

#endif