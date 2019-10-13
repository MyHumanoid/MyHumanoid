#include "Shader.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "GlInclude.h"

#include "Logger.h"
#include "Vfs.h"


static void compileShader(GLuint shaderId, std::string & shaderStr)
{
	const char * vertSrc = shaderStr.c_str();
	glShaderSource(shaderId, 1, &vertSrc, NULL);
	glCompileShader(shaderId);
	
	GLint result = GL_FALSE;
	int   logLength;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::string vertShaderError;
		vertShaderError.resize(logLength, 0);
		glGetShaderInfoLog(shaderId, logLength, NULL, vertShaderError.data());
		log_error("{}", vertShaderError);
	}
}



std::optional<mh::Shader> LoadShader(const char * vertex_path, const char * fragment_path)
{
	log_info("Compiling vertex shader");
	std::string vertShaderStr;
	vfs::loadString(vertex_path, vertShaderStr);
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertShader, vertShaderStr);

	log_info("Compiling fragment shader");
	std::string fragShaderStr;
	vfs::loadString(fragment_path, fragShaderStr);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragShader, fragShaderStr);
	
	log_info("Linking program");
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	
	GLint result = GL_FALSE;
	int   logLength;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if(logLength) {
		std::string programError;
		programError.resize(logLength, 0);
		glGetProgramInfoLog(program, logLength, NULL, programError.data());
		log_error("{}", programError);
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	if(result == GL_TRUE) {
		return mh::Shader(program);
	} else {
		return std::nullopt;
	}
}
