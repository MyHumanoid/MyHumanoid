#version 440

// Fixed function shading emulation

uniform sampler2D texUnit0;

layout(location = 0)
in vec4 v_frontColor;
layout(location = 1)
in vec2 v_texCoord0;

layout(location = 0)
out vec4 o_fragColor;

void main()
{
	vec4 color = v_frontColor;
	vec4 texture0Arg0 = texture2D(texUnit0, v_texCoord0);
	color = clamp(color * texture0Arg0, 0.0, 1.0);
	o_fragColor = color;
}
