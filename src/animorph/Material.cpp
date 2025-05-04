#include "animorph/Material.h"

#include "FileReader.h"
#include "Logger.h"

namespace Animorph
{

bool loadMaterials(MaterialVector & mv, const std::string & filename)
{
	FileReader in_stream;
	
	log_debug("Open File: {}", filename);
	if(!in_stream.open(filename))
		return false;
	
	mv.clear();

	std::string buffer;
	char  name[MAX_LINE_BUFFER];
	float c0, c1, c2 = 0.0;
	float alpha = 1.0;

	while(in_stream.getline(buffer)) {
		Material mat;
		Color    color;

		if(sscanf(buffer.c_str(), "%[^,],%f,%f,%f,%f\n", name, &c0, &c1, &c2, &alpha) == 5) {
			mat.name = name;
			color.rgba(c0, c1, c2, alpha);
			mat.color = color;

			mv.push_back(mat);
		} else {
			std::cerr << "illegal Material data format:" << std::endl << buffer << std::endl;
		};
	}
	return true;
}

}
