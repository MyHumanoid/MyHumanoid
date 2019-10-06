#include "animorph/TextureVector.h"

namespace Animorph
{

void TextureVector::fromUVStream(FileReader & in_stream)
{
	unsigned int    nr_uv_coord;
	int             n = 0;
	char            buffer[MAX_LINE_BUFFER];
	float           v[4], u[4];
	TextureVector & texture_vector = (*this);

	while(in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		nr_uv_coord = sscanf(buffer, "%f %f %f %f %f %f %f %f\n", &u[0], &v[0], &u[1],
		                     &v[1], &u[2], &v[2], &u[3], &v[3]);

		TextureFace texture_face;

		if(nr_uv_coord == 6) {
			texture_face.push_back(glm::vec2(u[0], v[0]));
			texture_face.push_back(glm::vec2(u[1], v[1]));
			texture_face.push_back(glm::vec2(u[2], v[2]));
		} else if(nr_uv_coord == 8) {
			texture_face.push_back(glm::vec2(u[0], v[0]));
			texture_face.push_back(glm::vec2(u[1], v[1]));
			texture_face.push_back(glm::vec2(u[2], v[2]));
			texture_face.push_back(glm::vec2(u[3], v[3]));
		}

		texture_vector.push_back(texture_face);

		n++;
	}
}

bool TextureVector::load(const string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromUVStream(file_reader);

	return true;
}

}

