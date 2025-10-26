#ifndef MGL_SIMULATION_HPP
#define MGL_SIMULATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

namespace mgl {

	/// <summary>
	/// Uniquely identifies an entity via an ID
	/// </summary>
	class Identifiable {
	public:
		Identifiable();
		GLuint getId();
	private:
		GLuint id;
		static GLuint idCounter;
	};
	
	/// <summary>
	/// Objects implementing this interface are assumed to require some
	/// time-based action. 
	/// </summary>
	class ITimeUpdateable : public Identifiable {
	public:
		ITimeUpdateable();
		virtual void update(GLfloat deltaTime) = 0;
	};

	/// <summary>
	/// Holds references to items that need to perform any
	/// time-based action. Passes the delta time to all
	/// items held once for each frame
	/// </summary>
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