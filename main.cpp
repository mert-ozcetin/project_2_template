#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <exception>
#include "main.h"
using namespace std;

vector<Person> people;
unordered_map<int, Bucket> dataMap;

namespace {
string trimCopy(const string& input) {
    const auto start = input.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    const auto end = input.find_last_not_of(" \t\r\n");
    return input.substr(start, end - start + 1);
}

void logRowError(size_t lineNumber, const string& field, const string& value) {
    cerr << "Warning: Skipping line " << lineNumber << " due to invalid "
         << field << " value '" << value << "'." << endl;
}

bool parseIntField(const string& token, int& destination) {
    try {
        size_t processed = 0;
        int value = stoi(token, &processed);
        if (processed != token.size()) {
            return false;
        }
        destination = value;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool parseDoubleField(const string& token, double& destination) {
    try {
        size_t processed = 0;
        double value = stod(token, &processed);
        if (processed != token.size()) {
            return false;
        }
        destination = value;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool parseBinaryBoolField(const string& token, bool& destination) {
    int value = 0;
    if (!parseIntField(token, value)) {
        return false;
    }
    if (value == 0) {
        destination = false;
        return true;
    }
    if (value == 1) {
        destination = true;
        return true;
    }
    return false;
}

bool parseGenderField(const string& token, bool& destination) {
    string normalized = token;
    transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c) {
        return static_cast<char>(tolower(c));
    });

    if (normalized == "male") {
        destination = true;
        return true;
    }
    if (normalized == "female") {
        destination = false;
        return true;
    }
    return false;
}
} // namespace

unordered_map<int, Bucket> loadData(const string& path) {
    unordered_map<int, Bucket> data;
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file at " << path << endl;
        return data;
    }

    string line;
    getline(file, line); // skip header
    size_t lineNumber = 1;

    while (getline(file, line)) {
        ++lineNumber;
        if (line.empty()) {
            continue;
        }

        vector<string> tokens;
        string token;
        stringstream lineStream(line);
        while (getline(lineStream, token, ',')) {
            tokens.push_back(trimCopy(token));
        }

        if (tokens.size() != 16) {
            cerr << "Warning: Skipping line " << lineNumber
                 << " due to unexpected column count (" << tokens.size() << ")." << endl;
            continue;
        }

        Person p;

        if (!parseIntField(tokens[0], p.id)) {
            logRowError(lineNumber, "id", tokens[0]);
            continue;
        }
        if (!parseIntField(tokens[1], p.age)) {
            logRowError(lineNumber, "age", tokens[1]);
            continue;
        }
        if (!parseGenderField(tokens[2], p.gender)) {
            logRowError(lineNumber, "gender", tokens[2]);
            continue;
        }
        if (!parseDoubleField(tokens[3], p.bmi)) {
            logRowError(lineNumber, "bmi", tokens[3]);
            continue;
        }
        if (!parseIntField(tokens[4], p.steps)) {
            logRowError(lineNumber, "daily_steps", tokens[4]);
            continue;
        }
        if (!parseDoubleField(tokens[5], p.sleepHrs)) {
            logRowError(lineNumber, "sleep_hours", tokens[5]);
            continue;
        }
        if (!parseDoubleField(tokens[6], p.waterIntake)) {
            logRowError(lineNumber, "water_intake_l", tokens[6]);
            continue;
        }
        if (!parseIntField(tokens[7], p.caloriesConsumed)) {
            logRowError(lineNumber, "calories_consumed", tokens[7]);
            continue;
        }
        if (!parseBinaryBoolField(tokens[8], p.smoker)) {
            logRowError(lineNumber, "smoker", tokens[8]);
            continue;
        }
        if (!parseBinaryBoolField(tokens[9], p.alcohol)) {
            logRowError(lineNumber, "alcohol", tokens[9]);
            continue;
        }
        if (!parseIntField(tokens[10], p.restingHeartRate)) {
            logRowError(lineNumber, "resting_hr", tokens[10]);
            continue;
        }
        if (!parseIntField(tokens[11], p.systolicBP)) {
            logRowError(lineNumber, "systolic_bp", tokens[11]);
            continue;
        }
        if (!parseIntField(tokens[12], p.diastolicBP)) {
            logRowError(lineNumber, "diastolic_bp", tokens[12]);
            continue;
        }
        if (!parseIntField(tokens[13], p.cholesterol)) {
            logRowError(lineNumber, "cholesterol", tokens[13]);
            continue;
        }
        if (!parseBinaryBoolField(tokens[14], p.famHistory)) {
            logRowError(lineNumber, "family_history", tokens[14]);
            continue;
        }
        if (!parseBinaryBoolField(tokens[15], p.atRisk)) {
            logRowError(lineNumber, "disease_risk", tokens[15]);
            continue;
        }

        people.push_back(p);

        // assign to buckets based on factor attributes
        (p.gender ? data[MALE] : data[FEMALE]).PerVec.push_back(p);
        (p.smoker ? data[SMOKER] : data[NON_SMOKER]).PerVec.push_back(p);
        (p.alcohol ? data[ALCOHOL] : data[NON_ALCOHOL]).PerVec.push_back(p);
        (p.famHistory ? data[FAMILY_HISTORY] : data[NO_FAMILY_HISTORY]).PerVec.push_back(p);
        (p.atRisk ? data[AT_RISK] : data[NOT_AT_RISK]).PerVec.push_back(p);

        //  add to linked lists for MergeSort
        (p.gender ? data[MALE] : data[FEMALE]).PerList.push_front(p);
    }

    file.close();
    return data;
}

void deleteId(int id, unordered_map<int, Bucket>& data) {
    for (auto& [key, bucket] : data) {
        bucket.idsVec.erase(remove(bucket.idsVec.begin(), bucket.idsVec.end(), id), bucket.idsVec.end());
        bucket.idsList.remove(id);
    }
}
