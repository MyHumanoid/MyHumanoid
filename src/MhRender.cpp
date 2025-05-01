#include "MhRender.h"

#include "GlInclude.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gui/CGUtilities.h"
#include "render/RenderUtils.h"
#include "Logger.h"
#include "render/Shader.h"
#include "Global.h"

#include "Profiler.h"

RenderBackground g_renderBackground;

void RenderBackground::init() {
	{
		glGenVertexArrays(1, &m_vertexArrayObject);
		glGenBuffers(1, &m_vertexBufferObject);
		
		glBindVertexArray(m_vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		
		static constexpr GLfloat bufferData[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			-1.0f,  1.0f,
			-1.0f,  1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
	}
	loadShader();
}

void RenderBackground::loadShader()
{
	log_info("Loading Background Shader");
	
	auto shader = LoadShader("data/shader/background.vert", "data/shader/background.frag");
	
	if(shader) {
		glDeleteProgram(m_shader->handle);
		m_shader = shader.value();
	}
}

void RenderBackground::render(AppState& app)
{
	int x, y;
	SDL_GetWindowSizeInPixels(app.mainWindow, &x, &y);
	
	glUseProgram(m_shader->handle);
	GLint myLoc = glGetUniformLocation(m_shader->handle, "u_viewportResolution");
	if(myLoc != -1) {
		glProgramUniform2f(m_shader->handle, myLoc, x, y);
	}
	
	glBindVertexArray(m_vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glUseProgram(0);
}

RenderBody g_renderBody;


struct Buffers {
	std::vector<glm::vec3> vert;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec4> color;
	std::vector<glm::vec2> texCoord;
	
	struct GlBuffers {
		GLuint vert;
		GLuint normal;
		GLuint color;
		GLuint texCoord;
	};
	GlBuffers glBuffers;
	
	GLuint m_vertexArrayObject;
	
	void init()
	{
		glGenBuffers(1, &glBuffers.vert);
		glGenBuffers(1, &glBuffers.normal);
		glGenBuffers(1, &glBuffers.color);
		glGenBuffers(1, &glBuffers.texCoord);
		
		glGenVertexArrays(1, &m_vertexArrayObject);
		glBindVertexArray(m_vertexArrayObject);
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.vert);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.normal);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.color);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.texCoord);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);
	}
	
	void clear()
	{
		vert.clear();
		normal.clear();
		color.clear();
		texCoord.clear();
	}
	
	void copy()
	{
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.vert);
		glBufferData(GL_ARRAY_BUFFER,
		             vert.size() * sizeof(glm::vec3),
		             vert.data(),
		             GL_DYNAMIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.normal);
		glBufferData(GL_ARRAY_BUFFER,
		             normal.size() * sizeof(glm::vec3),
		             normal.data(),
		             GL_DYNAMIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.color);
		glBufferData(GL_ARRAY_BUFFER,
		             color.size() * sizeof(glm::vec4),
		             color.data(),
		             GL_DYNAMIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, glBuffers.texCoord);
		glBufferData(GL_ARRAY_BUFFER,
		             texCoord.size() * sizeof(glm::vec2),
		             texCoord.data(),
		             GL_DYNAMIC_DRAW);
	}
	
	void render()
	{
		glBindVertexArray(m_vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, vert.size());
	}
};

static Buffers g_buffers;


void RenderBody::init()
{
	loadShader(1);
	g_buffers.init();
}

void RenderBody::loadShader(int version)
{
	log_info("Loading Shader set {}", version);
	
	std::optional<mh::Shader> shader;
	if(version == 1) {
		shader = LoadShader("data/shader/body.vert", "data/shader/body.frag");
	} else {
		shader = LoadShader("data/shader/body_2.vert", "data/shader/body_2.frag");
	}
	
	if(shader) {
		glDeleteProgram(m_shader->handle);
		m_shader = shader.value();
	}
}

void RenderBody::loadTexture()
{
	for(auto & [name, value] : g_mesh.facegroup().m_groups) {
		
		std::string fileName = "data/pixmaps/ui/" + name + "_color.png";
		value.texture        = LoadTextureFromFile(fileName.c_str());
		if(!value.texture) {
			log_error("couldn't load base skin_color Texture Data {}_color.png", name);
		}
		
		
		{
			std::string dir = "data/data/rib_data/textures_data/";
			value.specular  = LoadTextureFromFile(dir + name + "_specular.png");
			value.bump      = LoadTextureFromFile(dir + name + "_bump.png");
		}
	}
}

extern Matrix g_modelViewMatrix;
extern glm::mat4 g_projectionMatrix;

void RenderBody::render()
{
	ZoneScoped;
	TracyGpuZone("RenderBody");
	
	using Animorph::MaterialVector;
	using Animorph::TextureVector;
	using Animorph::FaceVector;
	using Animorph::Face;
	using Animorph::TextureFace;
	using Animorph::Material;
	using Animorph::Vertex;
	
	
	const MaterialVector & materialvector(g_mesh.materials());
	const TextureVector &  texturevector(g_mesh.textureVector());
	
	const FaceVector & facevector(g_mesh.faces());
	
	const VertexVector & vertexvector(g_mesh.getVertexVectorRef());
	
	
	cgutils::enableBlend();
	
	glUseProgram(m_shader->handle);
	
	glm::mat4 modelViewMatrix = glm::make_mat4(g_modelViewMatrix.data);
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
	
	glUniformMatrix4fv(glGetUniformLocation(m_shader->handle, "u_modelViewMatrix"),
	                   1, GL_FALSE, glm::value_ptr(modelViewMatrix));
	
	glUniformMatrix3fv(glGetUniformLocation(m_shader->handle, "u_NormalMatrix"),
	                   1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	glUniformMatrix4fv(glGetUniformLocation(m_shader->handle, "u_projectionMatrix"),
	                   1, GL_FALSE, glm::value_ptr(g_projectionMatrix));
	
	
	for(auto & [goupName, groupValue] : g_mesh.facegroup().m_groups) {
		
		if(groupValue.visible == false)
			continue;
		
		g_buffers.clear();
		
		if(g_global.m_enableTexture) {
			if(groupValue.texture) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, groupValue.texture.value().handle);
				glUniform1i(glGetUniformLocation(m_shader->handle, "texture0"),
				            0);
			}
			if(groupValue.specular) {
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, groupValue.specular.value().handle);
				glUniform1i(glGetUniformLocation(m_shader->handle, "texture1"),
				            1);
			}
			if(groupValue.bump) {
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, groupValue.bump.value().handle);
				glUniform1i(glGetUniformLocation(m_shader->handle, "texture2"),
				            2);
			}
		}
		
		for(const auto & faceIndex : groupValue.facesIndexes) {
			const Face &        face         = facevector[faceIndex];
			const TextureFace & texture_face = texturevector[faceIndex];
			
			glm::vec4 faceColor;
			
			int material_index = face.getMaterialIndex();
			if(material_index != -1) {
				const Material & material(materialvector[material_index]);
				const Color &    color(material.color);
				
				faceColor = glm::vec4(color.r, color.g, color.b, color.a);
			}
			
			auto pushVert = [&](int j) {
				const int & vertIndex = face.vertices[j];
				const Vertex & vertex = vertexvector.m_verts[vertIndex];
				const glm::vec3 & normal = vertexvector.m_normals[vertIndex];
				const glm::vec2 & uv = texture_face[j];
				
				g_buffers.vert.push_back(vertex.pos);
				g_buffers.normal.push_back(normal);
				g_buffers.color.push_back(faceColor);
				g_buffers.texCoord.push_back(uv);
			};
			
			pushVert(0);
			pushVert(1);
			pushVert(2);
			if(face.getSize() == 4) {
				pushVert(0);
				pushVert(2);
				pushVert(3);
			}
		}
		
		g_buffers.copy();
		g_buffers.render();
	}
	
	glUseProgram(0);
	
	glDisable(GL_BLEND);
}
