
uniform mat4 u_modelViewMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_projectionMatrix;

in vec3 i_Vertex;
in vec3 i_Normal;
in vec4 i_Color;
in vec2 i_TexCoord;

out vec3 io_vertexNormal;
out vec4 io_vertexColor;
out vec2 io_texCoord;
out vec3 io_FragPos;

void main(void)
{
	vec4 vert = vec4(i_Vertex, 1.0);
	
	gl_Position = u_projectionMatrix * u_modelViewMatrix * vert;
	
	io_FragPos = vec3(u_modelViewMatrix * vert);
	
	io_vertexNormal = normalize(u_NormalMatrix * i_Normal);
	io_vertexColor = i_Color;
	io_texCoord = i_TexCoord;
}
