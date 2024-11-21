#include <mgl/world/mglWorld.hpp>

namespace mgl {
	World& World::getInstance(void) {
		static World Instance;
		return Instance;
	}

	// TODO
	void World::init() {
		
	}
}