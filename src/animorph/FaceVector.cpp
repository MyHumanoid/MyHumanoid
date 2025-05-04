#include "animorph/FaceVector.h"

#include "Logger.h"

namespace Animorph
{

bool loadGeometry(FaceVector & fv, const std::string & filename)
{
	FileReader file_reader;
	
	log_debug("Open File: {}", filename);
	if(!file_reader.open(filename))
		return false;

	int nr_faces;
	
	fv.clear();
	
	std::string buffer;
	int  v0, v1, v2, v3;
	
	int line = 0;
	while(file_reader.getline(buffer)) {
		nr_faces = sscanf(buffer.c_str(), "%d,%d,%d,%d\n", &v0, &v1, &v2, &v3);
		line += 1;
		
		if(nr_faces == 3) {
			fv.push_back(Face(v0, v1, v2));
		} else if(nr_faces == 4) {
			fv.push_back(Face(v0, v1, v2, v3));
		} else {
			log_error("Impossible number of faces: {} in line {}", nr_faces, line);
			continue;
		}
	}

	return true;
}

bool loadColors(FaceVector & fv, const std::string & filename)
{
	FileReader file_reader;
	
	log_debug("Open File: {}", filename);
	if(!file_reader.open(filename))
		return false;

	int  n = 0;
	std::string buffer;
	int  mat_index;
	
	while(file_reader.getline(buffer)) {
		if(sscanf(buffer.c_str(), "%d\n", &mat_index) == 1) {
			if(n >= 0 && size_t(n) < fv.size()) {
				Face & face = fv.at(n);
				
				face.setMaterialIndex(mat_index);
			} else {
				log_error("Face with number {} doesn't exist!", n);
			}
			n++;
		}
	}
	
	
	return true;
}





}

