#include <mgl/scene/mglLightManager.hpp>
#include <mgl/camera/mglCameraManager.hpp>
namespace mgl {

	void LightManager::updateShaders(ShaderProgram& shaders) {
		this->forEach([&shaders](Light& light) {
			light.updateShaders(shaders);
		});
	}
}