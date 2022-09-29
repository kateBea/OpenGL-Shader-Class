#include <iostream>
#include <cstdint>
#include <array>
#include <string_view>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tools/shader.h"

int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize glfw\n";
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	constexpr std::int32_t width{ 640 };
	constexpr std::int32_t height{ 480 };

	GLFWwindow* window{ glfwCreateWindow(width, height, "shader_class", nullptr, nullptr) };

	if (!window)
	{
		std::cerr << "There was an error creating the window\n";
		glfwTerminate();
		return 1;
	}

	// Create a valid context for initializing glew (OpenGL)
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize glew!\n";
		glfwTerminate();
		return 1;
	}

	// shader paths in command line arguments
	std::cout << "vertex shader path: " << argv[1] << std::endl;
	std::cout << "pixel shader path: " << argv[2] << std::endl;

	Shader shaders(argv[1], argv[2]);

	float vertices[]{
		// Positions		// Colors
		0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	// Bottom Right
	   -0.5f, 0.5f, 0.0f,   0.0f, 0.4f, 0.0f,	// Bottom Left
		0.0f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f	// Top 
	};

	std::uint32_t vertexBufferObject{};
	std::uint32_t vertexArrayBuffer{};

	glGenVertexArrays(1, &vertexArrayBuffer);
	glGenBuffers(1, &vertexBufferObject);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(vertexArrayBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind VAO
	glBindVertexArray(0);

	std::int32_t viewPortWidth{};
	std::int32_t viewPortHeight{};

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Update viewport to resize triangle on window resizing
		glfwGetFramebufferSize(window, &viewPortWidth, &viewPortHeight);

		// tell OpenGL the size of the rendering window so OpenGL knows how we want 
		// to display the data and coordinates with respect
		// to the window. We set those dimensions via the glViewport function
		// specify whicch part of the screen we want OpenGL to render
		glViewport(0, 0, viewPortWidth, viewPortHeight);

		// Rendering
		glClearColor(0.7f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaders.use();

		// Draw the triangle
		glBindVertexArray(vertexArrayBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
	}

	// cleanup
	glDeleteVertexArrays(1, &vertexArrayBuffer);
	glDeleteBuffers(1, &vertexBufferObject);
	glfwTerminate();

	return 0;
}