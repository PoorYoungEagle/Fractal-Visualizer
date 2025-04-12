#include <iostream>
#include <algorithm>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <glm/vec4.hpp>

#include "shader.h"
#include "controls.h"
#include "gui.h"
#include "state.h"

int HEIGHT;
int OPENGL_WIDTH;
int IMGUI_WIDTH;

int iterations;
float contrast;
float escapeRadius;

std::vector<float> stopPositions;

std::vector<glm::vec4> colorStops;

float vertices[] = {
	-1.0, -1.0, 0.0,
	1.0, 1.0, 0.0,
	-1.0, 1.0, 0.0,
	1.0, -1.0, 0.0
};

int indices[] = {
	0, 1, 2,
	0, 3, 1
};

static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {

	if (!glfwInit()) {
		std::cout << "OpenGL / GLFW failed to initiate";
		return -1;
	};
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Fractal Visualizer", NULL, NULL);
	glfwMaximizeWindow(window);

	HEIGHT = mode->height;
	IMGUI_WIDTH = mode->width - mode->height;
	OPENGL_WIDTH = mode->width - IMGUI_WIDTH;

	setupControls(window);

	if (!window) {
		std::cout << "GLFW window failed to initiate";
		glfwTerminate();
		return -1;
	};

	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		std::cout << "GLAD failed to load";
		return -1;
	};

	glViewport(0, 0, OPENGL_WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setupGUI(window);

	Shader fractalShader;

	std::vector<float> pixelData(OPENGL_WIDTH * HEIGHT * 3, 0.0f);

	iterations = 100;
	contrast = 0.5f;
	escapeRadius = 5;

	stopPositions = { 0.0f, 0.3f, 0.7f, 1.0f };

	colorStops = {
		glm::vec4(0.0f, 0.01f, 0.28f, 1.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec4(0.29f, 0.32f, 0.69f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	};

	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, OPENGL_WIDTH, HEIGHT);

		glClearColor(0.003f, 0.04f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fractalShader.useShader();

		createFrame();
		componentsForGUI(fractalShader);
		renderFrame();

		fractalShader.setFloat("zoom", zoom);
		fractalShader.setFloat("centerX", centerX);
		fractalShader.setFloat("centerY", centerY);
		fractalShader.setFloat("iterations", iterations);

		fractalShader.setVec4Array("colorStops", colorStops);
		fractalShader.setFloatArray("stopPositions", stopPositions);
		fractalShader.setInt("iterations", iterations);
		fractalShader.setFloat("contrast", contrast);
		fractalShader.setFloat("escapeRadius", escapeRadius);

		fractalShader.setVec2("iResolution", glm::vec2(OPENGL_WIDTH, HEIGHT));

		for (auto& [name, control] : variableControls) {
			fractalShader.setVec2(name.c_str(), control.value);
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		glReadPixels(0, 0, OPENGL_WIDTH, HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, pixelData.data());

		exitWindow(window);
	}

	removeFrame();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}