// util.h

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>

using namespace std;

class util
{
public:
	static bool ReadFileToString(const char* fileName, string& outFile);
};