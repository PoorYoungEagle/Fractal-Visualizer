#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader {
public:

	unsigned int ID;
	Shader();
	~Shader();

	void useShader() const;
	void reload(std::vector<std::string> variables, std::string customEquation);

	void setFloat(const std::string& name, double value) const;
	void setInt(const std::string& name, int value) const;
	void setVec4(const std::string& name, glm::vec4 vec) const;
	void setVec2(const std::string& name, glm::vec2 vec) const;

	void setVec4Array(const std::string& name, const std::vector<glm::vec4>& values) const;
	void setFloatArray(const std::string& name, const std::vector<float>& values) const;

private:

	std::string readShaderFile(const char* filePath);
};