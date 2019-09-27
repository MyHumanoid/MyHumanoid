#include "animorph/EdgeStrip.h"
#include "animorph/util.h"

using namespace std;
using namespace Animorph;

bool EdgeStrip::load(const std::string &filename)
{
	FileReader file_reader;

	file_reader.open(filename);

	if (!file_reader)
		return false;

	fromStream(file_reader);

	return true;
}

void EdgeStrip::fromStream(std::ifstream &in_stream)
{
	clear();

	char buffer[MAX_LINE];
	while (in_stream.getline(buffer, MAX_LINE)) {
		StripData face_numbers;

		stringTokeni(buffer, ",", face_numbers);
		push_back(face_numbers);
	}
}
