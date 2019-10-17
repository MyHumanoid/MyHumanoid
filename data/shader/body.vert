#version 120

// Fixed function shading emulation

// See:
// https://github.com/mojocorp/ShaderGen
// https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php

uniform vec4 light_model_ambient = vec4(0.2, 0.2, 0.2, 1.0);

// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMaterial.xml
uniform vec4  mat_ambient   = vec4(0.7, 0.7, 0.7, 1.0);
uniform vec4  mat_diffuse   = vec4(0.8, 0.8, 0.8, 1.0);
uniform vec4  mat_specular  = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4  mat_emission  = vec4(0.0, 0.0, 0.0, 1.0);
uniform float mat_shininess = 100.0;

// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml
uniform vec4 light_position[2] = vec4[2](
	vec4( 1.0, 1.0, 1.0, 0.0),
	vec4(-1.0, 1.0, 1.0, 0.0)
);
uniform vec4 light_ambient[2] = vec4[2](
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0)
);
uniform vec4 light_diffuse[2] = vec4[2](
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0)
);
uniform vec4 light_specular[2] = vec4[2](
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0)
);

uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;

varying vec4 v_frontColor;
varying vec2 v_texCoord0;


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
	float nDotHV = max(0.0, dot(normal, halfVector));
	
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

void flight(in vec3 normal, in vec4 ecPosition)
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
	      + Diffuse  * gl_Color;
	      + Specular * mat_specular;
	
	v_frontColor = clamp(color, 0.0, 1.0);
}

void main(void)
{
	// Do fixed functionality vertex transform
	gl_Position = u_projectionMatrix * u_modelViewMatrix * gl_Vertex;
	
	vec3 transformedNormal = normalize(gl_NormalMatrix * gl_Normal);
	
	// Eye-coordinate position of vertex, needed in various calculations
	vec4 ecPosition = u_modelViewMatrix * gl_Vertex;
	flight(transformedNormal, ecPosition);
	
	v_texCoord0 = gl_MultiTexCoord0.xy;
}
