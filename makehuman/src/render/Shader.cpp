#include "Shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


std::string readFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	
	if(!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}
	
	std::string line = "";
	while(!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	
	fileStream.close();
	return content;
}


std::optional<GLuint> LoadShader(const char *vertex_path, const char *fragment_path) {
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shaders
	std::string vertShaderStr = readFile(vertex_path);
	std::string fragShaderStr = readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();
	
	GLint result = GL_FALSE;
	int logLength;
	
	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);
	
	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::vector<char> vertShaderError(logLength);
		glGetShaderInfoLog(vertShader, logLength, NULL, vertShaderError.data());
		std::cout << &vertShaderError[0] << std::endl;
	}
	
	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);
	
	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::vector<char> fragShaderError(logLength);
		glGetShaderInfoLog(fragShader, logLength, NULL, fragShaderError.data());
		std::cout << &fragShaderError[0] << std::endl;
	}
	
	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::vector<char> programError(logLength);
		glGetProgramInfoLog(program, logLength, NULL, programError.data());
		std::cout << &programError[0] << std::endl;
	}
	
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	
	if(result == GL_TRUE) {
		return program;
	} else {
		return std::nullopt;
	}
}
