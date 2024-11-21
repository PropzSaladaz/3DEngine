#ifndef MGL_CHUNK_HPP
#define MGL_CHUNK_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glad/glad.h>

namespace {
	class Chunk {

		// instead of sending a 3D floating point vec, for each vertex, send a byte representing its
		// normal
		const enum class FaceNormals : uint8_t {
			Top,
			Bottom,
			Right,
			left,
			Front, 
			Back,
			NumFaces
		};

	public:
		Chunk init();
		renderOpaque();

	private:
		unsigned int& VAO;
		std::vector<uint> vertices;
	};
}

#endif