#include "controls.h"

bool isDragging = false;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
double centerX = 0.0;
double centerY = 0.0;
float zoom = 1.0;

void setupControls(GLFWwindow* window) {
	glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
	glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	glfwSetMouseButtonCallback(window, mouseButtonInput);
	glfwSetScrollCallback(window, scrollInput);
	glfwSetCursorPosCallback(window, cursorPositionCallback);
}

void exitWindow(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void mouseButtonInput(GLFWwindow* window, int button, int action, int mods) {
	
	if (ImGui::GetIO().WantCaptureMouse) {
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
		return;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			isDragging = true;
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		}
		else if (action == GLFW_RELEASE) {
			isDragging = false;
		}
	}
}

void scrollInput(GLFWwindow* window, double xOffset, double yOffset) {

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (mouseX < OPENGL_WIDTH) {
		float zoomFactor = (yOffset > 0) ? 0.9f : 1.1f;
		zoom *= zoomFactor;
		zoom = std::max(0.000001f, std::min(zoom, 10.0f));

	}
}

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {

	if (isDragging && xPos >= 0) {
		double deltaX = xPos - lastMouseX;
		double deltaY = yPos - lastMouseY;

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		centerX -= (deltaX / OPENGL_WIDTH) * zoom;
		centerY += (deltaY / HEIGHT) * zoom;

		lastMouseX = xPos;
		lastMouseY = yPos;
	}
}