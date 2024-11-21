#include <mgl/mglSimulation.hpp>

namespace mgl {

	GLuint Identifiable::idCounter = 0;
	Identifiable::Identifiable() : id(++idCounter) {}
	GLuint Identifiable::getId() {
		return id;
	}

	ITimeUpdateable::ITimeUpdateable() : Identifiable() {}

	Simulation::Simulation() {}

	Simulation::~Simulation() {}

	Simulation& Simulation::getInstance() {
		static Simulation instance;
		return instance;
	}

	void Simulation::trackUpdateable(GLuint id, ITimeUpdateable* updateable) {
		updateables.insert(std::make_pair(id, updateable));
	}

	void Simulation::stopTracking(GLuint id) {
		updateables.erase(id);
	}

	void Simulation::update(GLfloat deltaTime) {
		for (const auto& pair : updateables) {
			pair.second->update(deltaTime);
		}
	}
}