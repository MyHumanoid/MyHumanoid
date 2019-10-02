/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "render/RenderUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include <vector>

#include <GL/glew.h>

#include "log/log.h"

#include <glm/glm.hpp>

#include "Vfs.h"


using IconMap = std::unordered_map<std::string, mh::Texture>;

static const stbi_io_callbacks adapter = {
    .read = [](void * user, char * data, int size)->int{
	    PHYSFS_File * f = static_cast<PHYSFS_File *>(user);
	    return PHYSFS_readBytes(f, data, size);
    },
    .skip = [](void * user, int n)->void{
	    PHYSFS_File * f = static_cast<PHYSFS_File *>(user);
	    auto pos = PHYSFS_tell(f);
	    PHYSFS_seek(f, pos + n);
    },
    .eof  = [](void * user)->int{
	    PHYSFS_File * f = static_cast<PHYSFS_File *>(user);
	    return PHYSFS_eof(f);
    }
};

std::optional<mh::Texture> LoadTextureFromFile(const std::string & fileName) {
	auto file = PHYSFS_openRead(fileName.c_str());
	if(file == nullptr) {
		auto errVal = PHYSFS_getLastErrorCode();
		auto errStr = PHYSFS_getErrorByCode(errVal);
		log_error("Failed to open file: {}, {}", fileName, errStr);
		return std::nullopt;
	}
	int64_t fileSize = PHYSFS_fileLength(file);
	if(fileSize < 0) {
		return std::nullopt;
	}
	
	glm::ivec2 size;
	auto img = stbi_load_from_callbacks(&adapter, file, &size.x, &size.y, NULL, 4);
	
	if(img == NULL) {
		log_error("Failed to load file: {}", fileName);
		return std::nullopt;
	}
	
	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);
	
	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Upload pixels into texture
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, img);
	stbi_image_free(img);
	
	//	*out_width = image_width;
	//	*out_height = image_height;
	
	if(!PHYSFS_close(file)) {
		log_error("Failed to close file: {}", fileName);
	}
	
	return mh::Texture(image_texture);
}

void loadTexturesFromDir(IconMap & target, const std::string & baseDir)
{
	
	auto dirs = vfs::list(baseDir, true);
	for(const auto & dir : dirs) {
		auto files = vfs::list(dir.c_str());
		
		
		for(const auto & file: files) {
			auto ret = LoadTextureFromFile(file.c_str());
			
			if(ret) {
				auto foo = file;
				foo.erase(0, baseDir.length() + 1);
				std::string foobar = vfs::removeExtension(foo);
	
				log_debug("Loaded {} as {}", std::string(file), std::string(foobar));
				target.insert(IconMap::value_type(foobar, ret.value()));
			} else {
				log_error("Failed to load file {}", file);
			}
		}
	}
}
