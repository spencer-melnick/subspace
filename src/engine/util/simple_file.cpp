#include "simple_file.hpp"

#include <fstream>
#include <exception>

#include <SDL.h>

using namespace std;
using namespace subspace;

string subspace::simpleFileRead(const string& filename) {

	//This is a really hacky solution to get platform-agnostic file loading. Please fix later --Stephen
	string basepath = SDL_GetBasePath();
    ifstream file(basepath.substr(0, basepath.rfind("subspace")) + "subspace/" + filename, ios::binary | ios::ate);
    
	auto size = file.tellg();

	if (static_cast<int>(size) == -1) {
		throw runtime_error("Could not find file: " + filename);
	}

    string result;
    result.resize(size);

    file.seekg(0, ios::beg);
    file.read(&result[0], size);

    return result;
}
