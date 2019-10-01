/* SPDX-License-Identifier: GPL-3.0-or-later */
#include "render/RenderUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include <experimental/filesystem>
#include <vector>

#include <GL/glew.h>

#include "log/log.h"


using IconMap = std::unordered_map<std::string, mh::Texture>;

std::optional<mh::Texture> LoadTextureFromFile(const std::string & file)
{
	const char * filename = file.c_str();

	// Load from file
	int             image_width  = 0;
	int             image_height = 0;
	unsigned char * image_data   = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if(image_data == NULL)
		return std::nullopt;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Upload pixels into texture
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
	             GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	//*out_texture = image_texture;
	//	*out_width = image_width;
	//	*out_height = image_height;

	return mh::Texture(image_texture);
}

std::vector<std::string> filesInDirRecursive(const fs::path & directoryPath)
{
	using Iter = fs::recursive_directory_iterator;

	std::vector<std::string> files;
	auto                     it  = Iter(directoryPath);
	auto                     end = Iter();
	while(it != end) {
		auto & path = it->path();
		if(fs::is_regular_file(path)) {
			files.push_back(path);
		}
		std::error_code ec;
		it.increment(ec);
		if(ec) {
			std::cerr << "Error While Accessing : " << path.string()
			          << " :: " << ec.message() << '\n';
		}
	}
	return files;
}



void loadTexturesFromDir(IconMap & target, const fs::path & baseDir)
{
	auto files = filesInDirRecursive(baseDir);

	for(auto & file : files) {
		auto ret = LoadTextureFromFile(file.c_str());

		if(ret) {
			auto foo = file;
			foo.erase(0, baseDir.string().length());
			fs::path foobar = foo;
			foobar.replace_extension();

			logger("Loaded {} as {}", std::string(file), std::string(foobar));
			target.insert(IconMap::value_type(foobar, ret.value()));
		} else {
			std::cout << fmt::format("Failed to load file {}\n", file) << std::endl;
		}
	}
}
