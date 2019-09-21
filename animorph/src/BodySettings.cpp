#include "../include/animorph/BodySettings.h"

#include <string.h>

using namespace std;
using namespace Animorph;

bool BodySettings::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

bool BodySettings::save(const std::string &filename)
{
	FileWriter file_writer;
	file_writer.open(filename);

	if (!file_writer)
		return false;

	std::ostringstream out_stream;
	createStream(out_stream);

	file_writer << out_stream.str();

	return true;
}

void BodySettings::fromStream(std::ifstream &in_stream)
{
	float bs_value;

	// empty map before load new settings
	clear();

	char buffer[MAX_LINE_BUFFER];
	char bs_name[MAX_LINE_BUFFER];
	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		if (sscanf(buffer, "%[^,],%f\n", bs_name, &bs_value) == 2) {
			if (bs_value != 0.0) {
				// remove "targets/" prefix in MH2.0 bodysettings
				// as side effect a sub directory with name "targets" is forbidden!!
				string bs_name_str = bs_name;
				if (bs_name_str.substr(0, strlen("targets/")) == "targets/") {
					bs_name_str.erase(0, strlen("targets/"));
				}

				float &value_ref = (*this)[bs_name_str];
				value_ref = bs_value;
			}
		} else {
			if (buffer[0] != '#') {
				cerr << "Not allowed line in BodySetting:" << endl << buffer << endl;
			} else {
				cursorPositions.push_back(string(buffer));
			}
		}
	}
}

void BodySettings::createStream(std::ostringstream &out_stream)
{
	for (BodySettings::iterator bodyset_it = begin(); bodyset_it != end();
	     bodyset_it++) {
		string name = (*bodyset_it).first;
		float morph_value = (*bodyset_it).second;
		out_stream << name << "," << morph_value << endl;
	}
}
