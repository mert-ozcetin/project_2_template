#include "mergesort.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace sorting {
namespace {

std::string toLowerCopy(const std::string& value) {
    std::string lower = value;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return lower;
}

std::string normalizeFieldName(const std::string& fieldName) {
    std::string normalized;
    normalized.reserve(fieldName.size());
    for (unsigned char ch : fieldName) {
        if (std::isspace(ch) || ch == '_') {
            continue;
        }
        normalized.push_back(static_cast<char>(std::tolower(ch)));
    }
    return normalized;
}
