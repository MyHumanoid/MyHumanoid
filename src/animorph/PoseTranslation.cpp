#include "animorph/PoseTranslation.h"

#include <cstdio>
#include "Logger.h"

#define FF_VERTEX_N 10


using std::multiset;

namespace Animorph
{

PoseTranslation::PoseTranslation()
        : target(new Target())
        , formFactor(1.0, 1.0, 1.0)
        , minAngle(0.0f)
        , maxAngle(0.0f)
        , normalize(false)
        ,
        // inFilename(),
        cat()
{
}

bool PoseTranslation::load(const std::string & filename)
{
	std::string infoFileName = filename + ".info";
	
	FileReader reader;
	if(!reader.open(infoFileName)) {
		return false;
	}
	
	std::string buffer;
	if(!reader.getline(buffer)) {
		return false;
	}
	sscanf(buffer.c_str(), "%f,%f,%f", &originalSize.x, &originalSize.y, &originalSize.z);
	
	if(!reader.getline(buffer)) {
		return false;
	}
	sscanf(buffer.c_str(), "%f,%f", &minAngle, &maxAngle);
	
	
	if(!target->load(filename))
		return false;

	return true;
}

void PoseTranslation::calcFormFactor(const VertexVector & vertexvector)
{
	multiset<float> minXSet, maxXSet;
	multiset<float> minYSet, maxYSet;
	multiset<float> minZSet, maxZSet;

	int   counter  = 0;
	int   n_vertex = FF_VERTEX_N;
	float minX = 0, maxX = 0;
	float minY = 0, maxY = 0;
	float minZ = 0, maxZ = 0;
	// pair<set<float>::iterator, bool> pr;

	Target & tmpTarget = getTarget();

	if(tmpTarget.size() < (FF_VERTEX_N * 2)) {
		n_vertex = (int)(tmpTarget.size() / 2);
	}

	for(Target::const_iterator target_it = tmpTarget.begin(); target_it != tmpTarget.end();
	    target_it++) {
		const TargetData & td(*target_it);
		if(counter < n_vertex) {
			minXSet.insert(vertexvector.m_verts[td.vertex_number].x);
			maxXSet.insert(vertexvector.m_verts[td.vertex_number].x);

			minYSet.insert(vertexvector.m_verts[td.vertex_number].y);
			maxYSet.insert(vertexvector.m_verts[td.vertex_number].y);

			minZSet.insert(vertexvector.m_verts[td.vertex_number].z);
			maxZSet.insert(vertexvector.m_verts[td.vertex_number].z);

			counter++;
		} else {
			if(vertexvector.m_verts[td.vertex_number].x < *(--minXSet.end())) {
				minXSet.insert(vertexvector.m_verts[td.vertex_number].x);
				minXSet.erase(--(minXSet.end()));
			}
			if(vertexvector.m_verts[td.vertex_number].x > *(maxXSet.begin())) {
				maxXSet.insert(vertexvector.m_verts[td.vertex_number].x);
				maxXSet.erase(maxXSet.begin());
			}

			if(vertexvector.m_verts[td.vertex_number].y < *(--minYSet.end())) {
				minYSet.insert(vertexvector.m_verts[td.vertex_number].y);
				minYSet.erase(--(minYSet.end()));
			}
			if(vertexvector.m_verts[td.vertex_number].y > *(maxYSet.begin())) {
				maxYSet.insert(vertexvector.m_verts[td.vertex_number].y);
				maxYSet.erase(maxYSet.begin());
			}

			if(vertexvector.m_verts[td.vertex_number].z < *(--minZSet.end())) {
				minZSet.insert(vertexvector.m_verts[td.vertex_number].z);
				minZSet.erase(--(minZSet.end()));
			}
			if(vertexvector.m_verts[td.vertex_number].z > *(maxZSet.begin())) {
				maxZSet.insert(vertexvector.m_verts[td.vertex_number].z);
				maxZSet.erase(maxZSet.begin());
			}
		}
	}

	for(const auto & it : minXSet) {
		minX += it;
	}
	for(const auto & it : maxXSet) {
		maxX += it;
	}

	for(const auto & it : minYSet) {
		minY += it;
	}
	for(const auto & it : maxYSet) {
		maxY += it;
	}

	for(const auto & it : minZSet) {
		minZ += it;
	}
	for(const auto & it : maxZSet) {
		maxZ += it;
	}

	double xsize = maxXSet.size();
	double ysize = maxYSet.size();
	double zsize = maxZSet.size();

	formFactor = glm::vec3((maxX / xsize - minX / xsize) / originalSize.x,
	                       (maxY / ysize - minY / ysize) / originalSize.y,
	                       (maxZ / zsize - minZ / zsize) / originalSize.z);
}

}
