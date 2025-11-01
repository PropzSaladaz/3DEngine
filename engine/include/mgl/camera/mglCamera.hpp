#pragma once

#include <mgl/mglTransform.hpp>

namespace mgl {

	struct PerspectiveParams {
		f32 fov;
		f32 screenRatio;
		f32 near;
		f32 far;
	};

	struct OrthoParams {
		f32 left;
		f32 right;
		f32 bottom;
		f32 top;
		f32 near;
		f32 far;
	};

	class Camera : Transform {
	public:
		Camera(i32 bindingPoint);

		static void orthoResize(OrthoParams* params, i32 winx, i32 winy);
		static void perspectiveResize(PerspectiveParams* params, i32 winx, i32 winy);

		void lookAtFrom(const Transform* target, const math::vec3& position);
		void lookAtFrom(const Transform* target, const Transform* _position);
		void lookAt(const Transform* target);
		void setPosition(f32 x, f32 y, f32 z);
		void setPosition(const math::vec3& newPos);
		void translate(f32 x, f32 y, f32 z);
		void translate(const math::vec3& translateDir);

		virtual void zoom(f32 step) = 0;
		void pitch(f32 degrees);
		void yaw(f32 degrees);
		void setActive();

		math::vec3 getFrontV() const;
		math::vec3 getRightV() const;
		math::vec3 getUpV() const;
		math::vec3 getPosition() const;
		math::mat4 getViewMatrix() const;

		virtual void updateScreenRatio(i32 width, i32 height) = 0;

	protected:

		ui32 uboId;
		i32 bindingPoint;

		math::mat4 viewMatrix;
		math::mat4 projectionMatrix;

		void updateViewMatrix();
		void computeViewMatrix();
		void setViewMatrix(const math::mat4& mat);
		void setViewMatrix();
		virtual void updateProjectionMatrix();
		virtual void computeProjectionMatrix() = 0;
		void setProjectionMatrix();
	};

}