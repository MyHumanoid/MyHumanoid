#include "Shader.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "GlInclude.h"

#include "Logger.h"
#include "Vfs.h"

std::optional<mh::Shader> LoadShader(const char * vertex_path, const char * fragment_path)
{
	std::string vertShaderStr;
	vfs::loadString(vertex_path, vertShaderStr);
	
	std::string  fragShaderStr;
	vfs::loadString(fragment_path, fragShaderStr);
	
	const char * vertShaderSrc = vertShaderStr.c_str();
	const char * fragShaderSrc = fragShaderStr.c_str();
	
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		
	GLint result = GL_FALSE;
	int   logLength;

	log_info("Compiling vertex shader");
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::vector<char> vertShaderError(logLength);
		glGetShaderInfoLog(vertShader, logLength, NULL, vertShaderError.data());
		log_error("{}", vertShaderError[0]);
	}

	log_info("Compiling fragment shader");
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::vector<char> fragShaderError(logLength);
		glGetShaderInfoLog(fragShader, logLength, NULL, fragShaderError.data());
		log_error("{}", fragShaderError[0]);
	}
	
	log_info("Linking program");
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::vector<char> programError(logLength);
		glGetProgramInfoLog(program, logLength, NULL, programError.data());
		log_error("{}", programError[0]);
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	if(result == GL_TRUE) {
		return mh::Shader(program);
	} else {
		return std::nullopt;
	}
}
