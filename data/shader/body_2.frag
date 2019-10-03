#version 140

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 io_vertexNormal;
in vec4 io_vertexColor;
in vec2 io_texCoord;
in vec3 io_FragPos;

out vec4 io_fragmentColor;


uniform vec4 light_model_ambient = vec4(0.2, 0.2, 0.2, 1.0);

uniform vec4  mat_emission  = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4  mat_ambient   = vec4(0.7, 0.7, 0.7, 1.0);
uniform vec4  mat_diffuse   = vec4(0.8, 0.8, 0.8, 1.0);
uniform vec4  mat_specular  = vec4(1.0, 1.0, 1.0, 1.0);
uniform float mat_shininess = 1.0;


vec4 Ambient;
vec4 Diffuse;
vec4 Specular;


void directionalLight(in int i, in vec3 normal)
{
	// normal . light direction
	float nDotVP = max(0.0, dot(normal, normalize(vec3(gl_LightSource[i].position))));
	// normal . light half vector
	float nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));
	
	float pf; // power factor
	if(nDotVP == 0.0) {
		pf = 0.0;
	} else {
		pf = pow(nDotHV, mat_shininess);
	}
	
	Ambient  += gl_LightSource[i].ambient;
	Diffuse  += gl_LightSource[i].diffuse  * nDotVP;
	Specular += gl_LightSource[i].specular * pf;
}

vec4 flight(in vec3 normal, in vec4 ecPosition)
{
	vec3 eye = vec3(0.0, 0.0, 1.0);
	
	Ambient  = vec4(0.0);
	Diffuse  = vec4(0.0);
	Specular = vec4(0.0);
	
	directionalLight(0, normal);
	//directionalLight(1, normal);
	
	vec4 sceneColor = mat_emission
	                + mat_ambient * light_model_ambient;
	
	vec4 color = sceneColor
	      + Ambient  * mat_ambient
	      + Diffuse  * io_vertexColor;
	      + Specular * mat_specular;
	
	return clamp(color, 0.0, 1.0);
}



void main()
{
	vec3 transformedNormal = normalize(gl_NormalMatrix * io_vertexNormal);
	
	vec4 color = flight(transformedNormal, vec4(io_FragPos, 1.0));
	
	vec4 texColor = texture2D(texture0, io_texCoord);
	
	color = clamp(color * texColor, 0.0, 1.0);
	gl_FragColor = color;
	
	
	//vec4 color = io_vertexColor;
	///color *= texture(texture0, io_texCoord);
	
	//io_fragmentColor = color;
}
