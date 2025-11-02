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

std::string trimCopy(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool parseInt(const std::string& token, int& destination) {
    try {
        size_t processed = 0;
        const int parsed = std::stoi(token, &processed);
        if (processed != token.size()) {
            return false;
        }
        destination = parsed;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool parseDouble(const std::string& token, double& destination) {
    try {
        size_t processed = 0;
        const double parsed = std::stod(token, &processed);
        if (processed != token.size()) {
            return false;
        }
        destination = parsed;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool parseBool(const std::string& token, bool& destination) {
    int numeric = 0;
    if (!parseInt(token, numeric)) {
        return false;
    }
    if (numeric == 0) {
        destination = false;
        return true;
    }
    if (numeric == 1) {
        destination = true;
        return true;
    }
    return false;
}

int compareGender(const std::string& lhs, const std::string& rhs) {
    const std::string lhsLower = toLowerCopy(lhs);
    const std::string rhsLower = toLowerCopy(rhs);
    if (lhsLower < rhsLower) {
        return -1;
    }
    if (lhsLower > rhsLower) {
        return 1;
    }
    return 0;
}
