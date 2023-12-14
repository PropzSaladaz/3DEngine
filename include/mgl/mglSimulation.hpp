#ifndef MGL_SIMULATION_HPP
#define MGL_SIMULATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

namespace mgl {

class ITimeUpdateable {
public:
	virtual void update(GLfloat deltaTime) = 0;
};


class Simulation {

public:
	Simulation();
	static Simulation& getInstance();
	void trackUpdateable(GLuint id, ITimeUpdateable* updateable);
	void stopTracking(GLuint id);
	void update(GLfloat deltaTime);
	Simulation(Simulation const&) = delete;
	virtual ~Simulation();
	void operator=(Simulation const&) = delete;

private:
	std::map<GLuint, ITimeUpdateable*> updateables;
};
}


#endif