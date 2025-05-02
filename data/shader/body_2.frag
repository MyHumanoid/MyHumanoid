
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 io_vertexNormal;
in vec4 io_vertexColor;
in vec2 io_texCoord;
in vec3 io_FragPos;

out vec4 o_fragmentColor;


vec4 light_model_ambient = vec4(0.2, 0.2, 0.2, 1.0);

vec4  mat_emission  = vec4(0.0, 0.0, 0.0, 1.0);
vec4  mat_ambient   = vec4(0.7, 0.7, 0.7, 1.0);
vec4  mat_diffuse   = vec4(0.8, 0.8, 0.8, 1.0);
vec4  mat_specular  = vec4(1.0, 1.0, 1.0, 1.0);
float mat_shininess = 1.0;

// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml
vec4 light_position[2] = vec4[2](
	vec4( 1.0, 1.0, 1.0, 0.0),
	vec4(-1.0, 1.0, 1.0, 0.0)
);
vec4 light_ambient[2] = vec4[2](
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0)
);
vec4 light_diffuse[2] = vec4[2](
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0)
);
vec4 light_specular[2] = vec4[2](
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0)
);


vec4 Ambient;
vec4 Diffuse;
vec4 Specular;


void directionalLight(in int i, in vec3 normal, in vec3 eye, in vec3 ecPosition3)
{
	// direction from surface to light position
	vec3 VP = vec3(light_position[i]) - ecPosition3;
	
	// Normalize the vector from surface to light position
	VP = normalize(VP);
	
	// direction of maximum highlights
	vec3 halfVector = normalize(VP + eye);
	
	
	// normal . light direction
	float nDotVP = max(0.0, dot(normal, normalize(vec3(light_position[i]))));
	// normal . light half vector
	float nDotHV = max(0.0, dot(normal, vec3(halfVector)));
	
	float pf; // power factor
	if(nDotVP == 0.0) {
		pf = 0.0;
	} else {
		pf = pow(nDotHV, mat_shininess);
	}
	
	Ambient  += light_ambient[i];
	Diffuse  += light_diffuse[i] * nDotVP;
	Specular += light_specular[i] * pf;
}

vec4 flight(in vec3 normal, in vec4 ecPosition)
{
	vec3 ecPosition3 = (vec3(ecPosition)) / ecPosition.w;
	
	vec3 eye = vec3(0.0, 0.0, 1.0);
	
	Ambient  = vec4(0.0);
	Diffuse  = vec4(0.0);
	Specular = vec4(0.0);
	
	directionalLight(0, normal, eye, ecPosition3);
	directionalLight(1, normal, eye, ecPosition3);
	
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
	vec4 color = flight(io_vertexNormal, vec4(io_FragPos, 1.0));
	
	vec4 texColor = texture(texture0, io_texCoord);
	
	color = clamp(color * texColor, 0.0, 1.0);
	o_fragmentColor = color;
	
	
	//vec4 color = io_vertexColor;
	///color *= texture(texture0, io_texCoord);
	
	//io_fragmentColor = color;
}
