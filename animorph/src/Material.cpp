#include "../include/animorph/Material.h"

#include <fstream>

using namespace std;
using namespace Animorph;

bool MaterialVector::loadMaterials(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void MaterialVector::fromStream(std::ifstream &in_stream)
{
	clear();

	char buffer[MAX_LINE_BUFFER];
	char name[MAX_LINE_BUFFER];
	float c0, c1, c2 = 0.0;
	float alpha = 1.0;

	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		Material mat;
		Color color;

		if (sscanf(buffer, "%[^,],%f,%f,%f,%f\n", name, &c0, &c1, &c2, &alpha) ==
		    5) {
			mat.setName(name);
			color.rgba(c0, c1, c2, alpha);
			mat.setRGBCol(color);

			(*this).push_back(mat);
		} else {
			cerr << "illegal Material data format:" << endl << buffer << endl;
		};
	}
}
