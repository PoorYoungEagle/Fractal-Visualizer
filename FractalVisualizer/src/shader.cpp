#include "shader.h"

const char* vertexShaderPath = "shaders/fractalVert.vert";
const char* fragmentShaderPath = "shaders/fractalFrag.frag";

Shader::Shader() {

	ID = glCreateProgram();

	// Vertex Shader

	std::string vertexShaderCode = readShaderFile(vertexShaderPath);
	const char* vertexShaderString = vertexShaderCode.c_str();
	int successVertex;
	char errorMessageVertex[512];

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderString, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &successVertex);
	if (!successVertex) {
		glGetShaderInfoLog(vertex, 512, nullptr, errorMessageVertex);
		std::cout << "Vertex\n";
		std::cout << "Error compiling shader: " << errorMessageVertex << "\n";
		std::cout << "Shader location: " << vertexShaderPath << "\n";
	}

	// Fragment Shader

	std::string fragmentShaderCode = readShaderFile(fragmentShaderPath);
	const char* fragmentShaderString = fragmentShaderCode.c_str();
	int successFragment;
	char errorMessageFragment[512];

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderString, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &successFragment);
	if (!successFragment) {
		glGetShaderInfoLog(fragment, 512, nullptr, errorMessageFragment);
		std::cout << "Fragment\n";
		std::cout << "Error compiling shader: " << errorMessageFragment << "\n";
		std::cout << "Shader location: " << fragmentShaderPath << "\n";
	}

	// Attach and Delete Shaders

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

Shader::~Shader() {
	if (ID != 0) {
		glDeleteProgram(ID);
		ID = 0;
	}
}

void Shader::useShader() const {
	glUseProgram(ID);
}


void Shader::reload(std::vector<std::string> variables, std::string customEquation) {
	glDeleteProgram(ID);
	ID = glCreateProgram();

	// Vertex Shader

	std::string vertexShaderCode = readShaderFile(vertexShaderPath);
	const char* vertexShaderString = vertexShaderCode.c_str();
	int successVertex;
	char errorMessageVertex[512];

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderString, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &successVertex);
	if (!successVertex) {
		glGetShaderInfoLog(vertex, 512, nullptr, errorMessageVertex);
		std::cout << "Vertex\n";
		std::cout << "Error compiling shader: " << errorMessageVertex << "\n";
		std::cout << "Shader location: " << vertexShaderPath << "\n";
	}

	// Fragment Shader

	std::string fragmentShaderCode = readShaderFile(fragmentShaderPath);

	// find placement uniform positions
	size_t uniformsPos = fragmentShaderCode.find("// [CUSTOM_UNIFORMS]");

	if (uniformsPos == std::string::npos) {
		std::cerr << "Error: Shader template missing markers!\n";
		return;
	}

	std::stringstream uniforms;
	std::unordered_set<std::string> definedVars;

	// add variable uniforms
	for (const auto& var : variables) {
		if (definedVars.find(var) == definedVars.end()) {
			uniforms << "uniform vec2 " << var << ";\n";
			definedVars.insert(var);
		}
	}

	size_t uniformsLineEnd = fragmentShaderCode.find('\n', uniformsPos);
	fragmentShaderCode.replace(uniformsPos, uniformsLineEnd - uniformsPos, uniforms.str());

	const std::string beginMarker = "// [BEGIN_CUSTOM_EQUATION]";
	const std::string endMarker = "// [END_CUSTOM_EQUATION]";

	size_t beginPos = fragmentShaderCode.find(beginMarker);
	size_t endPos = fragmentShaderCode.find(endMarker);

	if (beginPos == std::string::npos || endPos == std::string::npos) {
		std::cerr << "Can't find markers\n";
		return;
	}

	size_t replaceLength = endPos - beginPos + endMarker.length();

	std::stringstream newEquation;
	newEquation << beginMarker << "\n"
		<< "vec2 customEquation(vec2 z, vec2 c) {\n"
		<< "    return " << customEquation << ";\n"
		<< "}\n"
		<< endMarker;

	fragmentShaderCode.replace(beginPos, replaceLength, newEquation.str());

	const char* fragmentShaderString = fragmentShaderCode.c_str();
	int successFragment;
	char errorMessageFragment[512];

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderString, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &successFragment);
	if (!successFragment) {
		glGetShaderInfoLog(fragment, 512, nullptr, errorMessageFragment);
		std::cout << "Fragment\n";
		std::cout << "Error compiling shader: " << errorMessageFragment << "\n";
		std::cout << "Shader location: " << fragmentShaderPath << "\n";
	}

	// Attach and Delete Shaders

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

std::string Shader::readShaderFile(const char* filePath) {

	std::ifstream shaderFile(filePath);
	if (!shaderFile) {
		std::cerr << "File failed to open: " << filePath << std::endl;
		return "";
	}

	std::ostringstream shaderCode;
	shaderCode << shaderFile.rdbuf();
	return shaderCode.str();
}

// Functions for specific Data Types

void Shader::setFloat(const std::string& name, double value) const {
	
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {

	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec4(const std::string& name, glm::vec4 vec) const {

	glUniform4f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setVec2(const std::string& name, glm::vec2 vec) const {

	glUniform2f(glGetUniformLocation(ID, name.c_str()), vec.x, vec.y);
}

void Shader::setVec4Array(const std::string& name, const std::vector<glm::vec4>& values) const {
	for (size_t i = 0; i < values.size(); ++i) {
		std::string uniformName = name + "[" + std::to_string(i) + "]";
		glUniform4f(glGetUniformLocation(ID, uniformName.c_str()), values[i].x, values[i].y, values[i].z, values[i].w);
	}
}

void Shader::setFloatArray(const std::string& name, const std::vector<float>& values) const {
	for (size_t i = 0; i < values.size(); ++i) {
		std::string uniformName = name + "[" + std::to_string(i) + "]";
		glUniform1f(glGetUniformLocation(ID, uniformName.c_str()), values[i]);
	}
}