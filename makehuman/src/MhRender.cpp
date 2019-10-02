#include "MhRender.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gui/CGUtilities.h"
#include "render/RenderUtils.h"
#include "log/log.h"
#include "render/Shader.h"
#include "Global.h"


std::optional<mh::Shader> g_bodyShader;
std::optional<mh::Shader> g_backgroundShader;

extern float twopoints[6];

// function to find the xyz values of the bounding box
void calcMinMax(const glm::vec3 & coords)
{
	if(coords.x < twopoints[0]) {
		twopoints[0] = coords.x;
	} else if(coords.x > twopoints[3]) {
		twopoints[3] = coords.x;
	}
	
	if(coords.y < twopoints[1]) {
		twopoints[1] = coords.y;
	} else if(coords.y > twopoints[4]) {
		twopoints[4] = coords.y;
	}
	
	if(coords.z < twopoints[2]) {
		twopoints[2] = coords.z;
	} else if(coords.z > twopoints[5]) {
		twopoints[5] = coords.z;
	}
}

void loadTextures()
{
	for(auto & [name, value] : g_mesh.facegroup().m_groups) {
		
		std::string fileName = "pixmaps/ui/" + name + "_color.png";
		value.texture        = LoadTextureFromFile(fileName.c_str());
		if(!value.texture) {
			log_error("couldn't load base skin_color Texture Data {}_color.png", name);
		}
		
		
		{
			std::string dir = "data/rib_data/textures_data/";
			value.specular  = LoadTextureFromFile(dir + name + "_specular.png");
			value.bump      = LoadTextureFromFile(dir + name + "_bump.png");
		}
	}
}

void LoadBodyShader(int version)
{
	log_info("Loading Shader set {}", version);
	
	std::optional<mh::Shader> shader;
	if(version == 1) {
		shader = LoadShader("shader/body.vert", "shader/body.frag");
	} else {
		shader = LoadShader("shader/body_2.vert", "shader/body_2.frag");
	}
	
	if(shader) {
		glDeleteProgram(g_bodyShader->handle);
		g_bodyShader = shader.value();
	}
}

void LoadBackgroundShader()
{
	log_info("Loading Background Shader");
	
	auto shader = LoadShader("shader/background.vert", "shader/background.frag");
	
	if(shader) {
		glDeleteProgram(g_backgroundShader->handle);
		g_backgroundShader = shader.value();
	}
}


#include "gui/Window.h"

void drawBackground()
{
	auto inSize = g_mainWindow->getSize();
	
	glUseProgram(g_backgroundShader->handle);
	GLint myLoc = glGetUniformLocation(g_backgroundShader->handle, "u_viewportResolution");
	if(myLoc != -1) {
		glProgramUniform2f(g_backgroundShader->handle, myLoc, inSize.getWidth(),
		                   inSize.getHeight());
	}
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glBegin(GL_QUADS);
	// bottom
	glVertex2f(-1.0, -1.0);
	glVertex2f(1.0, -1.0);
	// top
	glVertex2f(1.0, 1.0);
	glVertex2f(-1.0, 1.0);
	glEnd();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glUseProgram(0);
}

// function which draws an axis aligned wireframe bounding box, given two
// points.
void RenderBoundingBox(const float twoxyz[6])
{
	float sx, sy, sz, tx, ty, tz; // scale and translate the glutWireCube(1)
	sx = twoxyz[3] - twoxyz[0];
	sy = twoxyz[4] - twoxyz[1];
	sz = twoxyz[5] - twoxyz[2];
	
	tx = twoxyz[0] + sx / 2;
	ty = twoxyz[1] + sy / 2;
	tz = twoxyz[2] + sz / 2;
	
	const Color box_color(0.8, 0.8, 0.8, 0.75);
	
	char strz[100], strx[100], stry[100];
	// conversions int to char[]. units?(these are not meters! we keep this value
	// anyway.)
	snprintf(strz, sizeof(strz), "D = %5.2f", sz * 10);
	snprintf(strx, sizeof(strx), "W = %5.2f", sx * 10);
	snprintf(stry, sizeof(stry), "H = %5.2f", sy * 10);
	
	cgutils::enableBlend();
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_LIGHTING);
	
	// Set the color for the bounding box
	::glColor4fv(box_color.getAsOpenGLVector());
	
	glPushMatrix();
	glTranslatef(tx, ty, tz);
	glScalef(sx, sy, sz);
	glutWireCube(1);
	glPopMatrix();
	
	glPushMatrix();
	
	// draws the D quote
	const glm::vec3 posz(twoxyz[3] + 0.5, twoxyz[4] + 0.5, tz);
	cgutils::drawString3D(posz, GLUT_BITMAP_9_BY_15, strz, box_color);
	
	// draws the W quote
	const glm::vec3 posx(tx - 0.5, twoxyz[4] + 0.75, twoxyz[5] + 0.5);
	cgutils::drawString3D(posx, GLUT_BITMAP_9_BY_15, strx, box_color);
	
	// draws the H
	const glm::vec3 posy(twoxyz[3] + 1.5, ty, twoxyz[5] + 0.5);
	cgutils::drawString3D(posy, GLUT_BITMAP_9_BY_15, stry, box_color);
	
	glPopMatrix();
	glFlush();
	
	glEnable(GL_LIGHTING);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
}

void renderMesh()
{
	const MaterialVector & materialvector(g_mesh.materials());
	const TextureVector &  texturevector(g_mesh.textureVector());
	
	const FaceVector & facevector(g_mesh.faces());
	
	const VertexVector & vertexvector(g_mesh.getVertexVectorRef());
	
	int facesize;
	int istri = -1; // to remember which type was the latest drawn geometry and
	    // avoid too many glBegin
	
	if(g_global.quotedBox) {
		for(int i = 0; i < 6; i++) {
			twopoints[i] = 0;
		}
	}
	
	cgutils::enableBlend();
	
	glUseProgram(g_bodyShader->handle);
	
	for(auto & [goupName, groupValue] : g_mesh.facegroup().m_groups) {
		
		if(groupValue.visible == false)
			continue;
		
		if(g_global.m_enableTexture) {
			if(groupValue.texture) {
				glActiveTexture(GL_TEXTURE0);
				::glBindTexture(GL_TEXTURE_2D, groupValue.texture.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture0"),
				            0);
			}
			if(groupValue.specular) {
				glActiveTexture(GL_TEXTURE1);
				::glBindTexture(GL_TEXTURE_2D, groupValue.specular.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture1"),
				            1);
			}
			if(groupValue.bump) {
				glActiveTexture(GL_TEXTURE2);
				::glBindTexture(GL_TEXTURE_2D, groupValue.bump.value().handle);
				glUniform1i(glGetUniformLocation(g_bodyShader->handle, "texture2"),
				            2);
			}
		}
		
		for(const auto & faceIndex : groupValue.facesIndexes) {
			const Face &        face         = facevector[faceIndex];
			const TextureFace & texture_face = texturevector[faceIndex];
			
			facesize = face.getSize();
			
			if(istri != facesize) {
				if(istri != -1) {
					::glEnd();
				}
				
				::glBegin(facesize == 4 ? GL_QUADS : GL_TRIANGLES);
				istri = facesize;
			}
			
			{
				unsigned int facesize = face.getSize();
				
				size_t j;
				
				int material_index = face.getMaterialIndex();
				if(material_index != -1) {
					const Material & material(materialvector[material_index]);
					const Color &    color(material.getRGBCol());
					
					// Set the color for these vertices
					::glColor4fv(color.getAsOpenGLVector());
				}
				
				for(j = 0; j != facesize; ++j) {
					const Vertex & vertex =
					    vertexvector[face.getVertexAtIndex(j)];
					const glm::vec2 & uv = texture_face[j];
					
					::glNormal3fv(glm::value_ptr(vertex.no));
					::glTexCoord2f(uv.x, uv.y);
					::glVertex3fv(glm::value_ptr(vertex.pos));
					
					if(g_global.quotedBox) {
						calcMinMax(vertex.pos);
					}
				}
			}
		}
		
		::glEnd();
		istri = -1;
	}
	
	glUseProgram(0);
	
	//	glColor3ub( 255, 255, 255 );
	//	for(const auto & strip: mesh->edgestrip) {
	//		glBegin(GL_LINE_STRIP);
	//		for(const auto & index: strip) {
	//			const Vertex &vertex = vertexvector[index];
	//			::glVertex3fv(glm::value_ptr(vertex.co));
	//		}
	//		glEnd();
	//	}
	
	glDisable(GL_BLEND);
}
