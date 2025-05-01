#pragma once

#include <optional>

#include "Global.h"
#include "GlInclude.h"
#include "render/GlTypes.h"

struct RenderBackground {
	
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	std::optional<mh::Shader> m_shader;
	
	void init();
	void loadShader();
	void render(AppState &app);
};

extern RenderBackground g_renderBackground;

struct RenderBody {
	std::optional<mh::Shader> m_shader;
	
	void init();
	void loadShader(int version);
	void loadTexture();
	void render();
};

extern RenderBody g_renderBody;
