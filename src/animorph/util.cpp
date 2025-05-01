#include "animorph/util.h"

using std::string;
using std::vector;

void Animorph::UtilStringDelFollow(std::string & str, const std::string & characters)
{
	string::size_type pos = str.find_last_not_of(characters);
	if(string::npos != pos)
		str = str.substr(0, pos + 1);
	else {
		// it is still possible that 'str' contains only 'characters':
		if(string::npos != str.find_first_of(characters))
			str.erase();
	}
}

void Animorph::UtilStringDelLead(std::string & str, const std::string & characters)
{
	string::size_type pos = str.find_first_not_of(characters);
	if(string::npos != pos)
		str = str.substr(pos);
	else {
		// it is still possible that 'str' contains only 'characters':
		if(string::npos != str.find_first_of(characters))
			str.erase();
	}
}

void Animorph::UtilStringDelSurround(std::string & str, const std::string & characters)
{
	string::size_type pos = str.find_first_not_of(characters);
	if(string::npos != pos)
		str = str.substr(pos);

	pos = str.find_last_not_of(characters);
	if(string::npos != pos)
		str = str.substr(0, pos + 1);
	else {
		// it is still possible that 'str' contains only 'characters':
		if(string::npos != str.find_first_of(characters))
			str.erase();
	}
}

bool Animorph::hasFileEnding(const std::string & filename, const std::string & ending)
{
	size_t loc = filename.find(ending, filename.length() - ending.length());

	if(loc != string::npos) {
		return true;
	}

	return false;
}

std::string Animorph::cutFileEnding(std::string filename, const std::string & ending)
{
	if(ending == "") {
		size_t loc = filename.find_last_of('.', filename.length());

		if(loc != string::npos) {
			filename.erase(loc);
			return filename;
		}
	} else {
		size_t loc = filename.find(ending, filename.length() - ending.length());

		if(loc != string::npos) {
			filename.erase(loc);
			return filename;
		}
	}

	return filename;
}

/*!
 * An function to replace occurences of substrings in a bigger string. Very
 * basic function without regex.
 * @param match This is the string that is matched to replace.
 * @param replace The string that replaces all found match strings.
 * @param str The string that is matched and replaced.
 * @param maxReplace Give a int to limit the replace matches. If 0 is given
 * there's no limit.
 */
int Animorph::replaceString(const string & match, const string & replace, string & str, size_t maxReplace)
{
	int          start = 0;
	unsigned int i     = 0;

	if(maxReplace == 0)
		maxReplace = str.length();

	for(; i < maxReplace; i++) {
		string::size_type loc = str.find(match, start);

		if(loc != string::npos) {
			str.replace(loc, match.length(), replace);
		} else {
			return i;
		}

		start = loc;
	}

	return i;
}

glm::vec3 Animorph::calcCenteroid(const vector<int> &  vertexNumbers,
                                  const VertexVector & vertexvector)
{
	glm::vec3 center = glm::vec3(0);

	for(vector<int>::const_iterator v_it = vertexNumbers.begin(); v_it != vertexNumbers.end();
	    v_it++) {
		int vn = *v_it;

		const Vertex & vertex(vertexvector.m_verts[vn]);
		center += vertex.pos;
	}

	if(vertexNumbers.size()) {
		center /= vertexNumbers.size();
	}

	return center;
}
