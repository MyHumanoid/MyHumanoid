#include "animorph/PoseSemiTarget.h"

#include "log/log.h"

namespace Animorph
{

PoseSemiTarget::PoseSemiTarget()
        : modVertex()
        , hasCenter(false)
{
}

bool PoseSemiTarget::load(const std::string & filename)
{
	char str[MAX_LINE_BUFFER];
	char tmp[MAX_LINE_BUFFER];
	char ax;
	char sign;

	clear();

	PoseSemiTarget & target(*this);

	int  ret = 0;
	bool rc  = true; // assume "success" by default

	// info file
	FILE * fd = fopen((filename + ".info").c_str(), "r");

	if(fd == NULL)
		return false;

	fgets(str, MAX_LINE_BUFFER, fd);

	if(str == NULL) // end of file reached?
		return false;

	fgets(tmp, MAX_LINE_BUFFER, fd);
	ret = sscanf(tmp, "%c", &ax);

	if(ret == EOF) // end of file reached?
		return false;

	fgets(tmp, MAX_LINE_BUFFER, fd);
	ret = sscanf(tmp, "%c", &sign);

	if(ret == EOF) // end of file reached?
		return false;

	fclose(fd);

	fd = fopen(filename.c_str(), "r");

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

		modVertex.insert(td.vertex_number);
		if(sign == '-') {
			td.rotation *= -1;
		}
		target.push_back(td);
	}

	fclose(fd);

	string vertexNumber(str);
	stringTokenize(vertexNumber, centerVertexNumbers);

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

void PoseSemiTarget::stringTokenize(const string & str, vector<int> & tokens)
{
	const string & delimiters = ", ";

	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(std::atoi(str.substr(lastPos, pos - lastPos).c_str()));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

}
