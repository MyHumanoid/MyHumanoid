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

	m_targetData.clear();

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
	
	{
		FileReader reader;
		log_debug("Open File: {}", filename);
		if(!reader.open(filename)) {
			return false;
		}
		
		std::string buffer;
		while(reader.getline(buffer)) {
			PoseTargetData td;
	
			int ret = sscanf(buffer.c_str(), "%d,%f", &td.vertex_number, &td.rotation);
	
			if((ret != 2) && (ret != 0)) {
				log_error("Illegal line while reading target '{}'!", filename);
				m_targetData.clear();
				return false;
			}
	
			modVertex.push_back(td.vertex_number);
			m_targetData.push_back(td);
		}
	}
	
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
	
	return rc;
}

}

