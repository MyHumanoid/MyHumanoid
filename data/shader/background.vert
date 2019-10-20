#version 450 core

layout(location = 0)
in vec2 i_Vertex;

void main()
{
	gl_Position = vec4(i_Vertex, 0.0, 1.0);
}
