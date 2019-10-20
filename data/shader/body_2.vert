#version 450 core

uniform mat4 u_modelViewMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_projectionMatrix;

layout(location = 0)
in vec3 i_Vertex;
layout(location = 1)
in vec3 i_Normal;
layout(location = 2)
in vec4 i_Color;
layout(location = 3)
in vec2 i_TexCoord;

layout(location = 0)
out vec3 o_vertexNormal;
layout(location = 1)
out vec4 o_vertexColor;
layout(location = 2)
out vec2 o_texCoord;
layout(location = 3)
out vec3 o_FragPos;

void main(void)
{
	vec4 vert = vec4(i_Vertex, 1.0);
	
	gl_Position = u_projectionMatrix * u_modelViewMatrix * vert;
	
	o_FragPos = vec3(u_modelViewMatrix * vert);
	
	o_vertexNormal = normalize(u_NormalMatrix * i_Normal);
	o_vertexColor = i_Color;
	o_texCoord = i_TexCoord;
}
