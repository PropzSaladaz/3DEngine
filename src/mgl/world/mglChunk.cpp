#include "mgl/world/mglChunk.hpp"

namespace mgl {
	Chunk::init() {
		float vertices[] = {
			//  positions         normals

			// front face
			0.0f, 0.0f, 0.0f,  //0.0f, 0.0f, 1.0f, // 0 bottom left
			1.0f, 0.0f, 0.0f,  //0.0f, 0.0f, 1.0f, // 1 bottom right
			1.0f, 1.0f, 0.0f,  //0.0f, 0.0f, 1.0f, // 2 top right
			0.0f, 1.0f, 0.0f,  //0.0f, 0.0f, 1.0f, // 3 top left

			// left face
			0.0f, 0.0f, 0.0f,  //0.0f, -1.0f, 0.0f, // 4 front bottom left
			0.0f, 1.0f, 0.0f,  //0.0f, -1.0f, 0.0f, // 5 front top left
			0.0f, 1.0f, -1.0f, //0.0f, -1.0f, 0.0f, // 6 back top left
			0.0f, 0.0f, -1.0f, //0.0f, -1.0f, 0.0f, // 7 back bottom left

			// right face
			1.0f, 0.0f, 0.0f,  //0.0f, 1.0f, 0.0f,  // 8 front bottom right
			1.0f, 0.0f, -1.0f, //0.0f, 1.0f, 0.0f,  // 9 back bottom right
			1.0f, 1.0f, -1.0f, //0.0f, 1.0f, 0.0f,  // 10 back top right
			1.0f, 1.0f, 0.0f,  //0.0f, 1.0f, 0.0f,  // 11 front top right

			// back face
			0.0f, 0.0f, -1.0f,  //0.0f, 0.0f, -1.0f, // 12 back bottom left
			0.0f, 1.0f, -1.0f,  //0.0f, 0.0f, -1.0f, // 13 back top left
			1.0f, 1.0f, -1.0f,  //0.0f, 0.0f, -1.0f, // 14 back top right
			1.0f, 0.0f, -1.0f,  //0.0f, 0.0f, -1.0f, // 15 back bottom right

			// bottom face
			0.0f, 0.0f, 0.0f,   //0.0f, -1.0f, 0.0f, // 16 front bottom left
			0.0f, 0.0f, -1.0f,  //0.0f, -1.0f, 0.0f, // 17 back bottom left
			1.0f, 0.0f, -1.0f,  //0.0f, -1.0f, 0.0f, // 18 back bottom right
			1.0f, 0.0f, 0.0f,   //0.0f, -1.0f, 0.0f, // 19 front bottom right

			// top face
			0.0f, 1.0f, 0.0f,   //0.0f, -1.0f, 0.0f, // 20 front top left
			1.0f, 1.0f, 0.0f,   //0.0f, -1.0f, 0.0f, // 21 front top right
			1.0f, 1.0f, -1.0f,  //0.0f, -1.0f, 0.0f, // 22 back top right
			0.0f, 1.0f, -1.0f,  //0.0f, -1.0f, 0.0f, // 23 back top left
		};

		// single cube
		unsigned int indices[]{
			0, 1, 2, 2, 3, 0, // front face
			4, 5, 6, 6, 7, 4, // left face
			8, 9, 10, 10, 11, 8, // right face
			12, 13, 14, 14, 15, 12, // back face
			16, 17, 18, 18, 19, 16, // bottom face
			20, 21, 22, 22, 23, 20, // top face
		};

		// generate & bind VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);


		unsigned int boIds[2];
		
		// generate & bind VBO
		glGenBuffers(1, &boIds[0]);
		glBindBuffer(GL_ARRAY_BUFFER, boIds[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// EBO
		glGenBuffers(1, &boIds[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boIds[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// configure attributes
		glVertexAttrbPointer(0, 3, GLFLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(2, boIds);
	}

	Chunk::renderOpaque() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

	}
}