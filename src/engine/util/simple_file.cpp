#include "simple_file.hpp"

#include <fstream>

using namespace std;
using namespace subspace;

string simpleFileRead(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    auto size = file.tellg();

    string result;
    result.resize(size);

    file.seekg(0, ios::beg);
    file.read(&result[0], size);

    return result;
}
