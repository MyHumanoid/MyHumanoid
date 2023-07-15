#version 450 core

// Fixed function shading emulation

uniform sampler2D texUnit0;

layout(location = 0)
in vec4 i_Color;
layout(location = 1)
in vec2 i_texCoord;

layout(location = 0)
out vec4 o_Color;

void main()
{
	vec4 color = i_Color;
	vec4 texture0Arg0 = texture(texUnit0, i_texCoord);
	color = clamp(color * texture0Arg0, 0.0, 1.0);
	o_Color = color;
}
