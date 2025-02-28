#include <fstream>

#include "Utils/ErrorHandling.hpp"
#include "Utils/Utils.hpp"

std::vector<char> utl::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw THROW_ERROR(("failed to open file " + filename).c_str());
    }
    const size_t fileSize = file.tellg();
    if (fileSize <= 0) {
        throw THROW_ERROR(("file " + filename + " is empty").c_str());
    }
    std::vector<char> buffer(fileSize);
    file.seekg(0, std::ios::beg);
    if (!file.read(buffer.data(), fileSize)) {
        throw THROW_ERROR(("failed to read file " + filename).c_str());
    }
    return buffer;
}
