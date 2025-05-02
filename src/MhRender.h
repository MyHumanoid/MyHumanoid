#pragma once

#include <optional>

#include "AbstractGl.h"
#include "Global.h"

struct RenderBackground {
	
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	std::optional<agl::Shader> m_shader;
	
	void init();
	void loadShader();
	void render(AppState &app);
};

extern RenderBackground g_renderBackground;

struct RenderBody {
	std::optional<agl::Shader> m_shader;
	
	void init();
	void loadShader(int version);
	void loadTexture();
	void render();
};

extern RenderBody g_renderBody;
