#include "render/DebugGl.h"

#include "GlInclude.h"
#include "Logger.h"

static void openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity,
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

void initDebugGl()
{
	log_info("Register OpenGL debug callback");
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	GLuint unusedIds = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
	
	std::string strInitialized("OpenGL debug output enabled");
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
	                     GL_DEBUG_TYPE_OTHER,
	                     1,
	                     GL_DEBUG_SEVERITY_LOW,
	                     GLsizei(strInitialized.size()), strInitialized.c_str());
}
