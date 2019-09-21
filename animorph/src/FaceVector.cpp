#include "../include/animorph/FaceVector.h"

using namespace std;
using namespace Animorph;

bool FaceVector::loadGeometry(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromGeometryStream(file_reader);

	return true;
}

bool FaceVector::loadColors(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromColorsStream(file_reader);

	return true;
}

void FaceVector::fromGeometryStream(std::ifstream &in_stream)
{
	int nr_faces;

	clear();

	char buffer[MAX_LINE_BUFFER];
	int v0, v1, v2, v3;

	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		nr_faces = sscanf(buffer, "%d,%d,%d,%d\n", &v0, &v1, &v2, &v3);

		if (nr_faces == 3) {
			push_back(Face(v0, v1, v2));
		} else if (nr_faces == 4) {
			push_back(Face(v0, v1, v2, v3));
		} else {
			cerr << "Impossible number of faces: " << nr_faces << endl;
			continue;
		}
	}
}

void FaceVector::fromColorsStream(std::ifstream &in_stream)
{
	int n = 0;
	char buffer[MAX_LINE_BUFFER];
	int mat_index;

	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		if (sscanf(buffer, "%d\n", &mat_index) == 1) {
			try {
				Face &face = (*this).at(n);

				face.setMaterialIndex(mat_index);
			} catch (const exception &e) {
				cerr << "Face with number " << n << " doesn't exist!" << endl;
				continue;
			}

			n++;
		}
	}
}
