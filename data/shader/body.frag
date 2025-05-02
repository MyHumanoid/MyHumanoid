// Fixed function shading emulation

uniform sampler2D texUnit0;

in vec4 io_Color;
in vec2 io_TexCoord;

out vec4 o_Color;


void main()
{
	vec4 color = io_Color;
	vec4 texture0Arg0 = texture(texUnit0, io_TexCoord);
	color = clamp(color * texture0Arg0, 0.0, 1.0);
	o_Color = color;
}
