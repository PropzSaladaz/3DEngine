#include <mgl/mglSimulation.hpp>
#include "types.hpp"
namespace mgl {

	ui32 Identifiable::idCounter = 0;
	Identifiable::Identifiable() : id(++idCounter) {}
	ui32 Identifiable::getId() {
		return id;
	}

	ITimeUpdateable::ITimeUpdateable() : Identifiable() {}

	Simulation::Simulation() {}

	Simulation::~Simulation() {}

	Simulation& Simulation::getInstance() {
		static Simulation instance;
		return instance;
	}

	void Simulation::trackUpdateable(ui32 id, ITimeUpdateable* updateable) {
		updateables.insert(std::make_pair(id, updateable));
	}

	void Simulation::stopTracking(ui32 id) {
		updateables.erase(id);
	}

	void Simulation::update(f32 deltaTime) {
		for (const auto& pair : updateables) {
			pair.second->update(deltaTime);
		}
	}
}