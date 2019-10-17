#version 440

uniform vec2 u_viewportResolution;

uniform vec4 topColor    = vec4(0.00, 0.0, 0.0, 1.0);
uniform vec4 middleColor = vec4(0.08, 0.1, 0.2, 1.0);
uniform vec4 bottomColor = vec4(0.00, 0.0, 0.0, 1.0);
uniform float h = 0.3;

layout(location = 0)
out vec4 o_fragColor;


// TODO attribute, from shadertoy, forgot the url
vec4 Dither_Vlachos(vec4 rgba, float levels) {
	// Vlachos 2016, "Advanced VR Rendering"
	vec3 noise = vec3(dot(vec2(171.0, 231.0), gl_FragCoord.xy));
	noise = fract(noise / vec3(103.0, 71.0, 97.0));
	noise = (noise * 2.0) - 1.0;
	return vec4(rgba.rgb + (noise / (levels - 1.0)), rgba.a);
}

void main(void)
{
	float y = gl_FragCoord.y / u_viewportResolution.y;
	
	vec4 mix1 = mix(bottomColor, middleColor, (y / h));
	vec4 mix2 = mix(middleColor, topColor,    (y - h)/(1.0 - h));
	vec4 col = mix(mix1, mix2, step(h, y));
	
	col = Dither_Vlachos(col, 255);
	
	o_fragColor = col;
}
