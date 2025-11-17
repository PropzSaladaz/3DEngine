#ifndef MGL_LIGHT_MANAGER_HPP
#define MGL_LIGHT_MANAGER_HPP

#include <mgl/mglManager.hpp>
#include <mgl/mglShaders.hpp>
#include <mgl/scene/mglLight.hpp>

namespace mgl {
	class LightManager : public Manager<Light>, public ShaderUpdator {
	public:
		void updateShaders(ShaderProgram& shaders) override;
	};
}

#endif