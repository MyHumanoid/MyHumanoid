#include "animorph/Material.h"

#include <fstream>

namespace Animorph
{

bool MaterialVector::loadMaterials(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromStream(file_reader);

	return true;
}

void MaterialVector::fromStream(FileReader & in_stream)
{
	clear();

	char  buffer[MAX_LINE_BUFFER];
	char  name[MAX_LINE_BUFFER];
	float c0, c1, c2 = 0.0;
	float alpha = 1.0;

	while(in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		Material mat;
		Color    color;

		if(sscanf(buffer, "%[^,],%f,%f,%f,%f\n", name, &c0, &c1, &c2, &alpha) == 5) {
			mat.name = name;
			color.rgba(c0, c1, c2, alpha);
			mat.color = color;

			(*this).push_back(mat);
		} else {
			std::cerr << "illegal Material data format:" << std::endl << buffer << std::endl;
		};
	}
}

}
