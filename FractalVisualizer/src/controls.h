#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdlib.h>
#include <algorithm>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"

#include "state.h"

void setupControls(GLFWwindow* window);
void exitWindow(GLFWwindow* window);
void mouseButtonInput(GLFWwindow* window, int button, int action, int mods);
void scrollInput(GLFWwindow* window, double xOffset, double yOffset);
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);

extern bool isDragging;
extern double lastMouseX;
extern double lastMouseY;

#endif // !CONTROLS_H