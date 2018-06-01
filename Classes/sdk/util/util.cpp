// util.cpp

#include "util.h"
#include <fstream>


bool util::ReadFileToString(const char * fileName, std::string & outFile)
{
	ifstream f(fileName);

	bool ret = false;

	if (f.is_open()) {
		string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		fprintf(stderr, "%s:%d: unable to open file `%s`\n", fileName);
	}

	return ret;
}
