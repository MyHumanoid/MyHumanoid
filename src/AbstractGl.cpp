// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2025 Eli2
#include "AbstractGl.h"

#include <SDL3/SDL.h>

#include "Logger.h"
#include "Vfs.h"

namespace agl {

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar * message, const void * userParam)
{
	const char * typeStr;
	switch(type) {
		case GL_DEBUG_TYPE_ERROR:
			typeStr = "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeStr = "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeStr = "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeStr = "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeStr = "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			typeStr = "OTHER";
			break;
	}
	
	const char * severityStr;
	switch(severity) {
		case GL_DEBUG_SEVERITY_LOW:
			severityStr = "LOW   ";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severityStr = "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			severityStr = "HIGH  ";
			break;
	}
	
	log_error("OpenGl:id{} {}: {} {}", id, severityStr, typeStr, message);
}

bool init() {
	int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
	if(version == 0) {
		return false;
	}
	
	log_info("GL   version: {}", (const char*)glGetString(GL_VERSION));
	log_info("GLSL version: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	
	log_info("Register OpenGL debug callback");
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugCallback, nullptr);
	GLuint unusedIds = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	
	std::string strInitialized("OpenGL debug output enabled");
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
	                     GL_DEBUG_TYPE_OTHER,
	                     1,
	                     GL_DEBUG_SEVERITY_LOW,
	                     GLsizei(strInitialized.size()), strInitialized.c_str());
	
	return true;
}

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



std::optional<agl::Shader> LoadShader(const char * vertex_path, const char * fragment_path)
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
		return agl::Shader(program);
	} else {
		return std::nullopt;
	}
}

} // namespace agl
