#include "animorph/PoseRotation.h"

#include "Logger.h"

namespace Animorph
{

PoseRotation::PoseRotation()
        : modVertex()
        , hasCenter(false)
        , minAngle(0.0f)
        , maxAngle(0.0f)
        , normalize(false)
        , cat()
{
}

bool PoseRotation::load(const std::string & filename)
{
	std::string vertexNumber;
	char ax;

	clear();

	PoseRotation & target(*this);

	int  ret = 0;
	bool rc  = true; // assume "success" by default
	
	{
		// info file
		std::string infoFile = filename + ".info";
		
		FileReader reader;
		if(!reader.open(infoFile)) {
			return false;
		}
		
		if(!reader.getline(vertexNumber)) {
			return false;
		}
		
		std::string buffer;
		if(!reader.getline(buffer)) {
			return false;
		}
		sscanf(buffer.c_str(), "%c", &ax);
		
		if(!reader.getline(buffer)) {
			return false;
		}
		sscanf(buffer.c_str(), "%f,%f", &minAngle, &maxAngle);
	}
	
	log_debug("Open File: {}", filename);
	FILE * fd = fopen(filename.c_str(), "r");

	if(fd == NULL)
		return false;

	// get the current locale
	char * locale = ::setlocale(LC_NUMERIC, NULL);

	// set it to "C"-Style ( the . (dot) means the decimal marker for floats)
	::setlocale(LC_NUMERIC, "C");

	for(;;) {
		PoseTargetData td;

		ret = fscanf(fd, "%d,%f", &td.vertex_number, &td.rotation);

		if(ret == EOF) // end of file reached?
			break;

		if((ret != 2) && (ret != 0)) {
			log_error("Illegal line while reading target '{}'!", filename);
			clear();
			rc = false; // mark the error
			break;
		}

		modVertex.push_back(td.vertex_number);

		target.push_back(td);
	}

	fclose(fd);

	//string vertexNumber(str);
	stringTokeni(vertexNumber, ", ", centerVertexNumbers);

	switch(ax) {
	case 'X':
		axis = X_AXIS;
		break;
	case 'Y':
		axis = Y_AXIS;
		break;
	case 'Z':
		axis = Z_AXIS;
		break;
	}

	// reset locale after file was written
	::setlocale(LC_NUMERIC, locale);

	return rc;
}

}

