#include "animorph/Target.h"

#include "log/log.h"

namespace Animorph
{

void Target::createStream(std::ostringstream & out_stream)
{
	for(Target::iterator target_it = begin(); target_it != end(); target_it++) {
		TargetData td = (*target_it);

		out_stream << td.vertex_number << "," << td.morph_vector.x << ","
		           << td.morph_vector.y << "," << td.morph_vector.z << "," << std::endl;
	}
}

bool Target::load(const std::string & filename)
{
	clear();

	FILE * fd = fopen(filename.c_str(), "r");
	if(!fd)
		return false;

	// get the current locale
	char * locale = ::setlocale(LC_NUMERIC, NULL);

	// set it to "C"-Style ( the . (dot) means the decimal marker for floats)
	::setlocale(LC_NUMERIC, "C");

	Target & target(*this);

	int  ret = 0;
	bool rc  = true; // assume "success" by default

	for(;;) {
		TargetData td;

		ret = fscanf(fd, "%d,%f,%f,%f", &td.vertex_number, &td.morph_vector.x,
		             &td.morph_vector.y, &td.morph_vector.z);

		if(ret == EOF) // end of file reached?
			break;

		if((ret != 4) && (ret != 0)) {
			
			log_error("Illegal line while reading target '{}'!", filename);
			clear();
			rc = false; // mark the error
			break;
		}

		target.push_back(td);
		modVertex.push_back(td.vertex_number);
	}

	// reset locale after file was written
	::setlocale(LC_NUMERIC, locale);

	fclose(fd);

	return rc;
}

bool Target::save(const std::string & filename)
{
	FileWriter file_writer;

	file_writer.open(filename);

	if(!file_writer)
		return false;

	std::ostringstream out_stream;
	createStream(out_stream);

	file_writer << out_stream.str();

	return true;
}

}
