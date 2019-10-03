#version 120

// Fixed function shading emulation

uniform sampler2D texUnit0;

void main (void) 
{
	vec4 color = gl_Color;
	vec4 texture0Arg0 = texture2D(texUnit0, gl_TexCoord[0].xy);
	color = clamp(color * texture0Arg0, 0.0, 1.0);
	gl_FragColor = color;
}
