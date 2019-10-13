#version 440

layout(location = 0) in vec2 vertexPos;

void main()
{
	gl_Position = vec4(vertexPos, 0.0, 1.0);
}
