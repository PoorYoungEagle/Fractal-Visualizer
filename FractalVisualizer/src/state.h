#pragma once

#ifndef STATE_H
#include <vector>
#include <unordered_map>
#include <string>
#include <array>
#include "glm/glm.hpp"

#define CONSTANTS_H

extern int OPENGL_WIDTH;
extern int HEIGHT;
extern int IMGUI_WIDTH;

extern int iterations;
extern float contrast;
extern float escapeRadius;

extern float zoom;
extern double centerX;
extern double centerY;

extern std::vector<float> stopPositions;
extern std::vector<glm::vec4> colorStops;

struct ComplexVariableControl {
    glm::vec2 value = { 0.0f, 0.0f };
    bool isConstant = false;
};

extern std::unordered_map<std::string, ComplexVariableControl> variableControls;

#endif // !STATE_H