#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <shaders.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void setVerticesForVAO(GLuint VAO, GLuint* VBO, GLuint size, GLfloat vertices[]) {
	glBindVertexArray(VAO);
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "App", NULL, NULL);
	if (window == NULL) {
		std::cout << "Couldnt create glfw window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ------------------
	// SHADERS
	// ------------------
	Shaders shaders1 = Shaders();
	shaders1.addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader1.vert", "VERTEX_SHADER");
	shaders1.addShader(GL_FRAGMENT_SHADER, "src/shaders/fragShader1.frag", "FRAGMENT_SHADER");

	Shaders shaders2 = Shaders();
	shaders2.addShader(GL_VERTEX_SHADER, "src/shaders/vertexShader1.vert", "VERTEX_SHADER");
	shaders2.addShader(GL_FRAGMENT_SHADER, "src/shaders/fragShader2.frag", "FRAGMENT_SHADER");


	GLuint program1 = shaders1.loadShaders();
	GLuint program2 = shaders2.loadShaders();

	// ------------------
	// VAOs
	// ------------------

	GLuint VAO[2];
	GLuint VBO[2];

	GLfloat vertices1[] = {
		 0.5f,  0.5f,   // top right
		 0.5f, -0.5f,   // bottom right
		-0.5f, -0.5f, // bottom left
	};

	GLfloat vertices2[] = {
		-0.5f,  -0.5f,   // top right
		0.5f, -0.5f,   // bottom right
		-0.5f, 0.5f, // bottom left
	};

	glGenVertexArrays(1, &VAO[0]);
	glGenVertexArrays(1, &VAO[1]);
	setVerticesForVAO(VAO[0], &VBO[0], sizeof(vertices1), vertices1);
	setVerticesForVAO(VAO[1], &VBO[1], sizeof(vertices2), vertices2);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (GLuint vao_i = 0; vao_i < 2; vao_i++) {
			// use different shaders program for each triangle
			if (vao_i % 2 == 0)
			{
				glUseProgram(program1);
				GLuint vertexColorLocation = glGetUniformLocation(program1, "vertexColor");
				GLfloat timeValue = glfwGetTime();
				GLfloat red = abs(sin(timeValue));
				GLfloat green = abs(sin(timeValue + 3.14 / 6));
				GLfloat blue = abs(sin(timeValue + +3.14 / 4));
				glUniform4f(vertexColorLocation, red, green, blue, 1.0f);
			}
			else
			{
				glUseProgram(program2);
			}

			glBindVertexArray(VAO[vao_i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}