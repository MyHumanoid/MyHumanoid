#include "animorph/FaceVector.h"

#include "Logger.h"

namespace Animorph
{

bool FaceVector::loadGeometry(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromGeometryStream(file_reader);

	return true;
}

bool FaceVector::loadColors(const std::string & filename)
{
	FileReader file_reader;

	if(!file_reader.open(filename))
		return false;

	fromColorsStream(file_reader);

	return true;
}

void FaceVector::fromGeometryStream(FileReader & in_stream)
{
	int nr_faces;

	clear();

	std::string buffer;
	int  v0, v1, v2, v3;
	
	int line = 0;
	while(in_stream.getline(buffer)) {
		nr_faces = sscanf(buffer.c_str(), "%d,%d,%d,%d\n", &v0, &v1, &v2, &v3);
		line += 1;

		if(nr_faces == 3) {
			push_back(Face(v0, v1, v2));
		} else if(nr_faces == 4) {
			push_back(Face(v0, v1, v2, v3));
		} else {
			log_error("Impossible number of faces: {} in line {}", nr_faces, line);
			continue;
		}
	}
}

void FaceVector::fromColorsStream(FileReader & in_stream)
{
	int  n = 0;
	std::string buffer;
	int  mat_index;

	while(in_stream.getline(buffer)) {
		if(sscanf(buffer.c_str(), "%d\n", &mat_index) == 1) {
			try {
				Face & face = (*this).at(n);

				face.setMaterialIndex(mat_index);
			} catch(const std::exception & e) {
				std::cerr << "Face with number " << n << " doesn't exist!" << std::endl;
				continue;
			}

			n++;
		}
	}
}

}

