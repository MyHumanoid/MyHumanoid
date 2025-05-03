// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2025 Eli2
#include "AbstractGl.h"

#include <SDL3/SDL.h>

#include "Logger.h"
#include "Vfs.h"

namespace agl {

#ifdef AGL_USE_GLES
	#define loadGl gladLoadGLES2
	#define glDebugMessageCallback glDebugMessageCallbackKHR
	#define glDebugMessageControl glDebugMessageControlKHR
	#define glDebugMessageInsert glDebugMessageInsertKHR

	#define GL_DEBUG_OUTPUT_SYNCHRONOUS GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR

	#define GL_DEBUG_SOURCE_APPLICATION GL_DEBUG_SOURCE_APPLICATION_KHR
	#define GL_DEBUG_SOURCE_SHADER_COMPILER GL_DEBUG_SOURCE_SHADER_COMPILER_KHR

	#define GL_DEBUG_TYPE_ERROR GL_DEBUG_TYPE_ERROR_KHR
	#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR
	#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR
	#define GL_DEBUG_TYPE_PORTABILITY GL_DEBUG_TYPE_PORTABILITY_KHR
	#define GL_DEBUG_TYPE_PERFORMANCE GL_DEBUG_TYPE_PERFORMANCE_KHR
	#define GL_DEBUG_TYPE_OTHER GL_DEBUG_TYPE_OTHER_KHR
	
	#define GL_DEBUG_SEVERITY_LOW GL_DEBUG_SEVERITY_LOW_KHR
	#define GL_DEBUG_SEVERITY_MEDIUM GL_DEBUG_SEVERITY_MEDIUM_KHR
	#define GL_DEBUG_SEVERITY_HIGH GL_DEBUG_SEVERITY_HIGH_KHR
#else
	#define loadGl gladLoadGL
#endif

void setWindowAttributes() {
	
	#ifdef AGL_USE_GLES
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#else
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	#endif
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 2);
}

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar * message, const void * userParam)
{
	if(source == GL_DEBUG_SOURCE_SHADER_COMPILER) {
		// We handle shader compiler errors explicitly
		return;
	}
	
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
	int version = loadGl((GLADloadfunc) SDL_GL_GetProcAddress);
	if(version == 0) {
		return false;
	}
	
	log_info("GL   version: {}", (const char*)glGetString(GL_VERSION));
	log_info("GLSL version: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	if(GLAD_GL_KHR_debug) {
		log_info("Gl debug supported");
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
	} else {
		log_info("Gl debug not supported :(");
	}
	
	return true;
}

static void compileShader(GLuint shaderId, const std::string& header, const std::string& shaderStr)
{
	const std::string foo = fmt::format("{}\n{}", header, shaderStr);
	const char * vertSrc = foo.c_str();
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
	#ifdef AGL_USE_GLES
		const char * headerFile = "data/shader/common_es.glsl";
	#else
		const char * headerFile = "data/shader/common_gl.glsl";
	#endif
	
	std::string header;
	vfs::loadString(headerFile, header);
	
	log_info("Compiling vertex shader");
	std::string vertShaderStr;
	vfs::loadString(vertex_path, vertShaderStr);
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertShader, header, vertShaderStr);
	
	log_info("Compiling fragment shader");
	std::string fragShaderStr;
	vfs::loadString(fragment_path, fragShaderStr);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragShader, header, fragShaderStr);
	
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
