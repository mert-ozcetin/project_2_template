#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Person {
    int id = 0;
    int age = 0;
    double bmi = 0;
    int steps = 0;
    double sleepHrs = 0;
    double waterIntake = 0;
    int caloriesConsumed = 0;
    int restingHeartRate = 0;
    int systolicBP = 0;
    int diastolicBP = 0;
    int cholesterol = 0;
    bool gender = false;
    bool smoker = false;
    bool alcohol = false;
    bool famHistory = false;
    bool atRisk = false;

    double risk() {
        double total  = 0.0;
        total += abs(20.0-age) * 0.01;
        total += abs(21.7-bmi) * 0.05;
        total += abs(3.7-waterIntake) * 0.05;
        total += abs(2000-caloriesConsumed) * 0.02;
        total += abs(80-restingHeartRate) * 0.03;
        total += abs(120.0-systolicBP) * 0.03;
        total += abs(80-diastolicBP) * 0.03;
        if (cholesterol >200) {
            total+= abs(cholesterol - 200)*0.03;
        }
        (total += smoker ? .5:0);
        (total += alcohol ? .5:0);
        (total += famHistory ? .5:0);
        (total += atRisk ? .5:0);
        return total;
    }

    bool operator==(const Person& other) const {
        return id == other.id;
    }
};

struct Bucket{
    vector<Person> PerVec;
    forward_list<Person> PerList;
};


enum BucketKey {
    MALE = 1,
    FEMALE = 2,
    SMOKER = 3,
    NON_SMOKER = 4,
    ALCOHOL = 5,
    NON_ALCOHOL = 6,
    FAMILY_HISTORY = 7,
    NO_FAMILY_HISTORY = 8,
    AT_RISK = 9,
    NOT_AT_RISK = 10
};

string trimCopy(const string& input) {
    const auto start = input.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    const auto end = input.find_last_not_of(" \t\r\n");
    return input.substr(start, end - start + 1);
}

void logRowError(size_t lineNumber, const string& field, const string& value) {
    cerr << "Warning: Skipping line " << lineNumber << " due to invalid " << field << " value '" << value << "'." << endl;
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
unordered_map<int, Bucket> loadData(string path){
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

        // assign to buckets based on factor attributes
        (p.gender ? data[MALE] : data[FEMALE]).PerVec.push_back(p);
        (p.smoker ? data[SMOKER] : data[NON_SMOKER]).PerVec.push_back(p);
        (p.alcohol ? data[ALCOHOL] : data[NON_ALCOHOL]).PerVec.push_back(p);
        (p.famHistory ? data[FAMILY_HISTORY] : data[NO_FAMILY_HISTORY]).PerVec.push_back(p);
        (p.atRisk ? data[AT_RISK] : data[NOT_AT_RISK]).PerVec.push_back(p);

        //  add to linked lists for MergeSort // finding the nth terms of male/female will be the only one merge sort will also be used for.
        (p.gender ? data[MALE] : data[FEMALE]).PerList.push_front(p);
    }

    file.close();
    return data;
};


vector<int> otherbucketshelper(Person p) {
    vector<int> otherbuckets;
    (p.smoker ? otherbuckets.push_back(3) : otherbuckets.push_back(4) );
    (p.alcohol ? otherbuckets.push_back(5) : otherbuckets.push_back(6) );
    (p.famHistory ? otherbuckets.push_back(7) : otherbuckets.push_back(8) );
    (p.atRisk ? otherbuckets.push_back(9) : otherbuckets.push_back(10) );
    return otherbuckets;
}

//first search through bucket 1 and 2 which are male and female, respeactively.
//determine which other buckets they are in and delete from data.
void deleteId(int id, unordered_map<int, Bucket>& data) {
    vector<int> othersbuck;
    bool flag = false;
    auto i = data[MALE].PerVec.begin();

    for (Person p : data[MALE].PerVec) {
        if (p.id == id) {
            flag = true;
            othersbuck = otherbucketshelper(p);
            data[MALE].PerVec.erase(i);
            break;
        }
        i++;
    }
    i = data[FEMALE].PerVec.begin();
    if (!flag) {
        for (Person p : data[FEMALE].PerVec) {
            if (p.id == id) {
                othersbuck = otherbucketshelper(p);
                data[FEMALE].PerVec.erase(i);
                break;
            }
            i++;
        }
    }

}

// not fully sure how we are going to implement this since the info would have to somehow be passed in
// would also have to be hashed and inserted into each respective bucket
void insertId(Person p, unordered_map<int, Bucket> &data){}

//similar to insert id with having to implement how we pass in the info as a param to the function.
//we just override the prexisting id's info.
void customizeId(int id, unordered_map<int, Bucket> &data, Person p){}

//generic display, we can just use male and female to find since they are guaranteed to be in one of them.
void display(int id, unordered_map<int, Bucket> &data){}

//our sorting algorithms - will sort based on risk factor not id.
void merge_sort(forward_list<int> &ids) {}
void quick_sort(vector<int> &ids) {}

//each function will use our sorting alg on each data structure and return the right value;
void nth_ind_highest(int n, unordered_map<int, Bucket> &data,int factor){}
void nth_ind_lowest(int n, unordered_map<int, Bucket> &data,int factor){}