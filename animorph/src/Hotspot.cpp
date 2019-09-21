#include "../include/animorph/Hotspot.h"

#include <string.h>

using namespace std;
using namespace Animorph;

bool Hotspot::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void Hotspot::fromStream(std::ifstream &in_stream)
{
	int vertex_number;

	clear();

	char buffer[MAX_LINE_BUFFER];
	string hs_name;
	while (in_stream.getline(buffer, MAX_LINE_BUFFER)) {
		if (isalpha(buffer[0])) // line is a hotspot identifier
		{
			// delete tailing ':'
			if (buffer[strlen(buffer) - 1] == ':')
				buffer[strlen(buffer) - 1] = '\0';

			hs_name = buffer;

			// force creation of empty Hotspots
			HotspotData iv = (*this)[hs_name];
		} else // line is a hotspot number
		{
			if (hs_name == "") {
				cerr << "There's something wrong in the hotspot file!" << endl;
				continue;
			}

			if (sscanf(buffer, "%d\n", &vertex_number) == 1) {
				HotspotData &iv = (*this)[hs_name];

				iv.push_back(vertex_number);
			}
		}
	}
}
