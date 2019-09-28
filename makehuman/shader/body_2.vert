#version 140

out vec3 io_vertexNormal;
out vec4 io_vertexColor;
out vec2 io_texCoord;
out vec3 io_FragPos;

void main(void)
{
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	
	io_FragPos = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	io_vertexNormal = gl_Normal;
	io_vertexColor = gl_Color;
	io_texCoord = gl_MultiTexCoord0.xy;
}
