#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <stack>


#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "state.h"
#include "shader.h"
#include "complexParser.h"

struct NamedEquation {
    const char* label;
    const char* expression;
};

void setupGUI(GLFWwindow* window);
void createFrame();
void renderFrame();
void removeFrame();

void componentsForGUI(Shader& fractalShader);

std::string translateEquationToGLSL(const std::string& equation);
std::vector<std::string> extractVariables(const std::string& equation);
void updateVariableExistence(const std::vector<std::string>& variables);
void createVariableSliders();
void applyEquationTwice(Shader& fractalShader, const char* equation);

#endif // !GUI_H