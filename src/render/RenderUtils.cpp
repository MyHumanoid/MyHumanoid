/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "render/RenderUtils.h"

#include <stb_image.h>

#include "GlInclude.h"

#include <vector>
#include <glm/glm.hpp>
#include <physfs.h>

#include "Logger.h"
#include "StringUtils.h"
#include "Vfs.h"


static const char * physfsError() {
	auto errVal = PHYSFS_getLastErrorCode();
	return PHYSFS_getErrorByCode(errVal);
}

struct AdapterUserdata {
	PHYSFS_File * file;
	const std::string & name;
};

static constexpr stbi_io_callbacks adapter = {
	[](void * user, char * data, int size) -> int {
		AdapterUserdata * d = static_cast<AdapterUserdata *>(user);
		auto res = PHYSFS_readBytes(d->file, data, size);
		if(res == -1) {
			log_error("Failed to read file: {}, {}", d->name, physfsError());
			return 0;
		}
		return res;
	},
	[](void * user, int n) -> void {
		AdapterUserdata * d = static_cast<AdapterUserdata *>(user);
		auto pos = PHYSFS_tell(d->file);
		if(pos == -1) {
			log_error("Failed to tell file: {}, {}", d->name, physfsError());
		}
		auto err = PHYSFS_seek(d->file, pos + n);
		if(err == 0) {
			log_error("Failed to seek file: {}, {}", d->name, physfsError());
		}
	},
	[](void * user) -> int {
		AdapterUserdata * d = static_cast<AdapterUserdata *>(user);
		return PHYSFS_eof(d->file);
	}
};

std::optional<mh::Texture> LoadTextureFromFile(const std::string & fileName) {
	auto file = PHYSFS_openRead(fileName.c_str());
	if(file == nullptr) {
		log_error("Failed to open file: {}, {}", fileName, physfsError());
		return std::nullopt;
	}
	int64_t fileSize = PHYSFS_fileLength(file);
	if(fileSize < 0) {
		return std::nullopt;
	}
	
	AdapterUserdata userdata {
		file,
		fileName
	};
	
	glm::ivec2 size;
	auto img = stbi_load_from_callbacks(&adapter, &userdata, &size.x, &size.y, NULL, 4);
	
	if(img == NULL) {
		log_error("Failed to load file: {}, {}", fileName, physfsError());
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
		log_error("Failed to close file: {}, {}", fileName, physfsError());
	}
	
	return mh::Texture(image_texture);
}

void loadTexturesFromDir(IconMap & target, const std::string & baseDir)
{
	auto dirs = vfs::list(baseDir, true);
	for(const auto & dir : dirs) {
		auto files = vfs::list(dir);
		
		for(const auto & file: files) {
			auto ret = LoadTextureFromFile(file);
			
			if(!ret) {
				log_error("Failed to load file {}", file);
				continue;
			}
			
			auto foo = file;
			foo.erase(0, baseDir.length() + 1);
			std::string foobar = removeExtension(foo);

			log_debug("Loaded {} as {}", file, foobar);
			target.insert(IconMap::value_type(foobar, ret.value()));
		}
	}
}

std::optional<Surface> loadSurfaceFromFile(const std::string & fileName) {
	auto file = PHYSFS_openRead(fileName.c_str());
	if(file == nullptr) {
		log_error("Failed to open file: {}, {}", fileName, physfsError());
		return std::nullopt;
	}
	int64_t fileSize = PHYSFS_fileLength(file);
	if(fileSize < 0) {
		return std::nullopt;
	}

	AdapterUserdata userdata {
		file,
		fileName
	};

	glm::ivec2 size;
	auto img = stbi_load_from_callbacks(&adapter, &userdata, &size.x, &size.y, NULL, STBI_rgb_alpha);

	if(img == NULL) {
		log_error("Failed to load file: {}, {}", fileName, physfsError());
		return std::nullopt;
	}

	int depth = 32;
	int pitch = 4 * size.y;
	const auto surface = SDL_CreateRGBSurfaceWithFormatFrom(img, size.x, size.y,
	                                                        depth, pitch,
	                                                        SDL_PIXELFORMAT_RGBA32);
	if(surface == nullptr) {
		log_error("Failed to create sdl surface");
		stbi_image_free(img);
	}

	if(!PHYSFS_close(file)) {
		log_error("Failed to close file: {}, {}", fileName, physfsError());
	}

	return Surface(surface, img);
}


Surface::Surface(SDL_Surface *ptr, unsigned char *img)
    : m_img(img)
    , m_ptr(ptr)
{}

void Surface::free() {
	SDL_FreeSurface(m_ptr);
	stbi_image_free(m_img);
}
