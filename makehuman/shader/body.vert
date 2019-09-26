#version 120

// Fixed function shading emulation

// See:
// https://github.com/mojocorp/ShaderGen
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php

uniform vec4 light_model_ambient = vec4(0.2, 0.2, 0.2, 1.0);

uniform vec4  mat_emission  = vec4(0.0, 0.0, 0.0, 1.0);
uniform vec4  mat_ambient   = vec4(0.7, 0.7, 0.7, 1.0);
uniform vec4  mat_diffuse   = vec4(0.8, 0.8, 0.8, 1.0);
uniform vec4  mat_specular  = vec4(1.0, 1.0, 1.0, 1.0);
uniform float mat_shininess = 100.0;


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

void flight(in vec3 normal, in vec4 ecPosition)
{
	vec3 eye = vec3(0.0, 0.0, 1.0);
	
	Ambient  = vec4(0.0);
	Diffuse  = vec4(0.0);
	Specular = vec4(0.0);
	
	directionalLight(0, normal);
	directionalLight(1, normal);
	
	vec4 sceneColor = mat_emission
	                + mat_ambient * light_model_ambient;
	
	vec4 color = sceneColor
	      + Ambient  * mat_ambient
	      + Diffuse  * gl_Color;
	      + Specular * mat_specular;
	
	gl_FrontColor = clamp(color, 0.0, 1.0);
}

void main(void)
{
	// Do fixed functionality vertex transform
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	
	vec3 transformedNormal = normalize(gl_NormalMatrix * gl_Normal);
	// Eye-coordinate position of vertex, needed in various calculations
	vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;
	flight(transformedNormal, ecPosition);
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_TexCoord[2] = gl_MultiTexCoord2;
	gl_TexCoord[3] = gl_MultiTexCoord3;
	gl_TexCoord[4] = gl_MultiTexCoord4;
}
