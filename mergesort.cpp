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

SortField parseSortField(const std::string& fieldName) {
    const std::string normalized = normalizeFieldName(fieldName);
    if (normalized == "id") {
        return SortField::Id;
    }
    if (normalized == "age") {
        return SortField::Age;
    }
    if (normalized == "gender") {
        return SortField::Gender;
    }
    if (normalized == "bmi" || normalized == "bodymassindex") {
        return SortField::BodyMassIndex;
    }
    if (normalized == "dailysteps") {
        return SortField::DailySteps;
    }
    if (normalized == "sleephours") {
        return SortField::SleepHours;
    }
    if (normalized == "waterintake" || normalized == "waterintakel") {
        return SortField::WaterIntake;
    }
    if (normalized == "calories" || normalized == "caloriesconsumed") {
        return SortField::CaloriesConsumed;
    }
    if (normalized == "smoker") {
        return SortField::Smoker;
    }
    if (normalized == "alcohol") {
        return SortField::Alcohol;
    }
    if (normalized == "restinghr" || normalized == "restingheartrate") {
        return SortField::RestingHeartRate;
    }
    if (normalized == "systolicbp" || normalized == "systolicbloodpressure") {
        return SortField::SystolicBloodPressure;
    }
    if (normalized == "diastolicbp" || normalized == "diastolicbloodpressure") {
        return SortField::DiastolicBloodPressure;
    }
    if (normalized == "cholesterol") {
        return SortField::Cholesterol;
    }
    if (normalized == "familyhistory") {
        return SortField::FamilyHistory;
    }
    if (normalized == "diseaserisk" || normalized == "risk") {
        return SortField::DiseaseRisk;
    }
    throw std::invalid_argument("Unknown field name: " + fieldName);
}

Comparator makeComparator(SortField field, bool descending) {
    return makeComparatorImpl(field, descending);
}

Comparator makeComparator(const std::string& fieldName, bool descending) {
    return makeComparatorImpl(parseSortField(fieldName), descending);
}

std::vector<HealthRecord> loadHealthRecords(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open CSV file: " + csvPath);
    }

    std::vector<HealthRecord> records;
    std::string line;
    if (!std::getline(file, line)) {
        return records;
    }

    std::size_t lineNumber = 1;
    while (std::getline(file, line)) {
        ++lineNumber;
        if (line.empty()) {
            continue;
        }

        std::stringstream lineStream(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(lineStream, token, ',')) {
            tokens.push_back(trimCopy(token));
        }
        if (tokens.size() != 16) {
            continue;
        }

        HealthRecord record;
        bool valid = true;
        valid &= parseInt(tokens[0], record.id);
        valid &= parseInt(tokens[1], record.age);
        record.gender = tokens[2];
        valid &= parseDouble(tokens[3], record.bmi);
        valid &= parseInt(tokens[4], record.dailySteps);
        valid &= parseDouble(tokens[5], record.sleepHours);
        valid &= parseDouble(tokens[6], record.waterIntakeLiters);
        valid &= parseInt(tokens[7], record.caloriesConsumed);
        valid &= parseBool(tokens[8], record.smoker);
        valid &= parseBool(tokens[9], record.alcohol);
        valid &= parseInt(tokens[10], record.restingHeartRate);
        valid &= parseInt(tokens[11], record.systolicBloodPressure);
        valid &= parseInt(tokens[12], record.diastolicBloodPressure);
        valid &= parseInt(tokens[13], record.cholesterol);
        valid &= parseBool(tokens[14], record.familyHistory);
        valid &= parseBool(tokens[15], record.diseaseRisk);

        if (valid) {
            records.push_back(std::move(record));
        }
    }

    return records;
}

void mergeSort(std::vector<HealthRecord>& records, const Comparator& comparator) {
    if (records.size() <= 1) {
        return;
    }
    std::vector<HealthRecord> buffer(records.size());
    mergeSortImpl(records, buffer, 0, records.size(), comparator);
}

std::vector<HealthRecord> mergeSortTopN(const std::vector<HealthRecord>& records, SortField field, bool descending, std::size_t topN) {
    std::vector<HealthRecord> sorted = records;
    mergeSort(sorted, makeComparator(field, descending));
    if (sorted.size() > topN) {
        sorted.resize(topN);
    }
    return sorted;
}

std::vector<HealthRecord> mergeSortTopN(const std::string& csvPath, SortField field, bool descending, std::size_t topN) {
    const auto records = loadHealthRecords(csvPath);
    return mergeSortTopN(records, field, descending, topN);
}

std::vector<HealthRecord> mergeSortTopN(const std::string& csvPath, const std::string& field, bool descending, std::size_t topN) {
    return mergeSortTopN(csvPath, parseSortField(field), descending, topN);
}
