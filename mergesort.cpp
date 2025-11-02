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

int compareRecords(const HealthRecord& lhs, const HealthRecord& rhs, SortField field) {
    auto compareIntegral = [](auto a, auto b) {
        if (a < b) {
            return -1;
        }
        if (a > b) {
            return 1;
        }
        return 0;
    };

    switch (field) {
        case SortField::Id:
            return compareIntegral(lhs.id, rhs.id);
        case SortField::Age:
            return compareIntegral(lhs.age, rhs.age);
        case SortField::Gender:
            return compareGender(lhs.gender, rhs.gender);
        case SortField::BodyMassIndex:
            return compareIntegral(lhs.bmi, rhs.bmi);
        case SortField::DailySteps:
            return compareIntegral(lhs.dailySteps, rhs.dailySteps);
        case SortField::SleepHours:
            return compareIntegral(lhs.sleepHours, rhs.sleepHours);
        case SortField::WaterIntake:
            return compareIntegral(lhs.waterIntakeLiters, rhs.waterIntakeLiters);
        case SortField::CaloriesConsumed:
            return compareIntegral(lhs.caloriesConsumed, rhs.caloriesConsumed);
        case SortField::Smoker:
            return compareIntegral(lhs.smoker, rhs.smoker);
        case SortField::Alcohol:
            return compareIntegral(lhs.alcohol, rhs.alcohol);
        case SortField::RestingHeartRate:
            return compareIntegral(lhs.restingHeartRate, rhs.restingHeartRate);
        case SortField::SystolicBloodPressure:
            return compareIntegral(lhs.systolicBloodPressure, rhs.systolicBloodPressure);
        case SortField::DiastolicBloodPressure:
            return compareIntegral(lhs.diastolicBloodPressure, rhs.diastolicBloodPressure);
        case SortField::Cholesterol:
            return compareIntegral(lhs.cholesterol, rhs.cholesterol);
        case SortField::FamilyHistory:
            return compareIntegral(lhs.familyHistory, rhs.familyHistory);
        case SortField::DiseaseRisk:
            return compareIntegral(lhs.diseaseRisk, rhs.diseaseRisk);
    }
    return 0;
}

Comparator makeComparatorImpl(SortField field, bool descending) {
    return [field, descending](const HealthRecord& lhs, const HealthRecord& rhs) {
        const int comparison = compareRecords(lhs, rhs, field);
        if (comparison == 0) {
            return lhs.id < rhs.id;
        }
        if (descending) {
            return comparison > 0;
        }
        return comparison < 0;
    };
}

void mergeSortImpl(std::vector<HealthRecord>& records, std::vector<HealthRecord>& buffer, std::size_t left, std::size_t right, const Comparator& comparator) {
    if (right - left <= 1) {
        return;
    }
    const std::size_t mid = left + (right - left) / 2;
    mergeSortImpl(records, buffer, left, mid, comparator);
    mergeSortImpl(records, buffer, mid, right, comparator);

    std::size_t i = left;
    std::size_t j = mid;
    std::size_t k = left;
    while (i < mid && j < right) {
        if (comparator(records[i], records[j])) {
            buffer[k++] = records[i++];
        } else {
            buffer[k++] = records[j++];
        }
    }
    while (i < mid) {
        buffer[k++] = records[i++];
    }
    while (j < right) {
        buffer[k++] = records[j++];
    }
    std::copy(buffer.begin() + static_cast<std::ptrdiff_t>(left), buffer.begin() + static_cast<std::ptrdiff_t>(right), records.begin() + static_cast<std::ptrdiff_t>(left));
}
