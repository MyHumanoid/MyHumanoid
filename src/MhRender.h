#pragma once

#include "GlInclude.h"

struct RenderBackground {
	
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	
	void init();
	void loadShader();
	void render();
};

extern RenderBackground g_renderBackground;


void loadTextures();

void LoadBodyShader(int version);

void RenderBoundingBox(const float twoxyz[6]);

void renderMesh();
