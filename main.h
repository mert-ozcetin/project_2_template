#include <string>
#include <iostream>
#include<iomanip>
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
    int steps = 0;
    int caloriesConsumed = 0;
    int restingHeartRate = 0;
    int systolicBP = 0;
    int diastolicBP = 0;
    int cholesterol = 0;

    double sleepHrs = 0;
    double waterIntake = 0;
    double bmi = 0;

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

vector<string> const factors{
    "male","female","smoker","non-smoker", "alcohol" , "no alcohol", "family history", "no family history"," at risk","not at risk"
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
    if (id ==-1 ) {
        return;
    }
    vector<int> othersbuck;
    bool flag = false;
    auto i = data[MALE].PerVec.begin();
    int count = 0;
    for (Person p : data[MALE].PerVec) {
        if (p.id == id) {
            flag = true;
            othersbuck = otherbucketshelper(p);
            data[MALE].PerVec.erase(i);
            data[MALE].PerList.remove(p);
            count++;
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
                data[FEMALE].PerList.remove(p);
                count++;
                break;
            }
            i++;
        }
    }
    for (int factor : othersbuck) {
        i = data[factor].PerVec.begin();
        for (Person p : data[factor].PerVec) {
            if (p.id == id) {
                data[factor].PerVec.erase(i);
                count++;
                break;
            }
            i++;
        }
    }
    cout << "ID deleted in " << count << " buckets" << endl;
}

// not fully sure how we are going to implement this since the info would have to somehow be passed in
// would also have to be hashed and inserted into each respective bucket
bool checkid(int id,unordered_map<int,Bucket> &data) {
    for (auto it : data[MALE].PerVec) {
        if (id == it.id) {
            cout <<"cannot insert, ID already present" <<endl;
             return false;
        }
    }
    for (auto it : data[FEMALE].PerVec) {
        if (id == it.id) {
            cout <<"cannot insert, ID already present" <<endl;
            return false;
        }
    }
    return true;
}

void insertPerson(Person p, unordered_map<int, Bucket> &data) {
    vector<int> othersbuck;
    othersbuck = otherbucketshelper(p);
    // if (!checkid(p.id,data)) {
    //     return;
    // }

    if (p.gender) {
        data[MALE].PerVec.push_back(p);
        data[MALE].PerList.push_front(p);
    }
    else {
        data[FEMALE].PerVec.push_back(p);
        data[FEMALE].PerList.push_front(p);
    }

    for (int factor : othersbuck) {
        data[factor].PerVec.push_back(p);
    }
    cout << "ID inserted" << endl;
}

//similar to insert id with having to implement how we pass in the info as a param to the function.
//we just override the prexisting id's info.
void customizeId(int id, unordered_map<int, Bucket> &data, Person p) {
    deleteId(id, data);
    insertPerson(p, data);
}

void printStats(Person p) {
    int const spacing = 30;
    cout << "ID: " << p.id <<endl;
    cout <<"-----------------------------------" <<endl;
    cout << left<< setw(spacing) <<"age: " << p.age << endl;
    cout << left<< setw(spacing) <<"bmi: " << p.bmi << endl;
    cout << left<< setw(spacing) <<"steps: " << p.steps << endl;
    cout << left<< setw(spacing) <<"sleep hours (hours): " << p.sleepHrs << endl;
    cout << left<< setw(spacing) <<"water intake (liters): " << p.waterIntake << endl;
    cout << left<< setw(spacing) <<"calories consumed: " << p.caloriesConsumed << endl;
    cout << left<< setw(spacing) <<"resting heart rate: " << p.restingHeartRate << endl;
    cout << left<< setw(spacing) <<"systolic bp: " << p.systolicBP << endl;
    cout << left<< setw(spacing) <<"diastolic bp: " << p.diastolicBP << endl;
    cout << left<< setw(spacing) <<"cholesterol (mg): " << p.cholesterol << endl;
    cout << left<< setw(spacing) <<"gender: " << (p.gender ? "male" : "female") << endl;
    cout << left<< setw(spacing) <<"smoker: " << (p.smoker ? "yes" : "no") << endl;
    cout << left<< setw(spacing) <<"alcohol: " << (p.alcohol ? "yes" : "no") << endl;
    cout <<left << setw(spacing) <<"family history:  " << (p.famHistory ? "yes" : "no")<< endl;
    cout <<left << setw(spacing) <<"at risk:  " << (p.atRisk ? "yes" : "no")<< endl;
    cout << left<< setw(spacing) <<"Risk value: " << p.risk() << endl << endl;
}

//generic display, we can just use male and female to find since they are guaranteed to be in one of them.
void display(int id, unordered_map<int, Bucket> &data) {
    for (Person p : data[MALE].PerVec) {
        if (p.id == id) {
            printStats(p);
            return;
        }
    }
    for (Person p : data[FEMALE].PerVec) {
        if (p.id == id) {
            printStats(p);
            return;
        }
    }
    cout << "ID not found" <<endl;
}

forward_list<Person> split(forward_list<Person> &records) {
    forward_list<Person> result;
    if (records.empty() || next(records.begin())==records.end()) {return result;};

    auto sp = records.begin();
    auto fp = records.begin();
    auto prev = records.before_begin();

    while (fp!=records.end() && next(fp)!=records.end()) {
        prev = sp;
        ++sp;
        fp = next(next(fp));
    }
    result.splice_after(result.before_begin(),records,prev,records.end());
    return result;
}

forward_list<Person> merge_list(forward_list<Person> &left,forward_list<Person> &right,bool ascending) {
    forward_list<Person> result;
    auto last = result.before_begin();

    if (ascending) {
        while (!left.empty() && !right.empty()) {
            if ( left.front().risk() <= right.front().risk()) {
                result.splice_after(last,left,left.before_begin());
                last = next(last);
            }
            else {
                result.splice_after(last,right,right.before_begin());
                last = next(last);
            }
        }
        if (!left.empty()) {result.splice_after(last,left);}
        if (!right.empty()) {result.splice_after(last,right);}
        return result;
    }
    else {
        while (!left.empty() && !right.empty()) {
            if ( left.front().risk() >= right.front().risk()) {
                result.splice_after(last,left,left.before_begin());
                last = next(last);
            }
            else {
                result.splice_after(last,right,right.before_begin());
                last = next(last);
            }
        }
        if (!left.empty()) {result.splice_after(last,left);}
        if (!right.empty()) {result.splice_after(last,right);}
        return result;
    }

}

//our sorting algorithms - will sort based on risk factor not id.
forward_list<Person> merge_sort(forward_list<Person> records,bool ascending) {
    if (records.empty() || next(records.begin())==records.end()) {
        return records;
    }
    auto right = split(records);
    auto left_sort = merge_sort(records,ascending);
    auto right_sort = merge_sort(right,ascending);
    return merge_list(left_sort,right_sort,ascending);
}

int medianof3(vector<Person> &records,int left,int right, bool ascending) {
    int mid = left + (right-left)/2;
    double leftval = records[left].risk();
    double rightval = records[right].risk();
    double midval = records[mid].risk();

    if (ascending) {
        if (leftval > midval) { swap(records[mid],records[left]); swap(midval,leftval);}
        if (rightval > midval) {swap(records[right],records[mid]); swap(rightval,midval);}
        if (leftval > midval) { swap(records[mid],records[left]); swap(midval,leftval);}
    }
    else {
        if (leftval < midval) { swap(records[mid],records[left]); swap(midval,leftval);}
        if (rightval < midval) {swap(records[right],records[mid]); swap(rightval,midval);}
        if (leftval < midval) { swap(records[mid],records[left]); swap(midval,leftval);}
    }
    return mid;
}

void quick_sort(vector<Person> &records,int left,int right, bool ascending) {
    if (ascending) {
        if (left >= right) return;


        int pivind = medianof3(records,left,right,ascending);
        swap (records[pivind],records[right]);
        double pivot = records[right].risk();

        int i = left;
        for (int j = left; j < right; j++) {
            if (records[j].risk() < pivot) {
                swap(records[i++], records[j]);
            }
        }
        swap(records[i], records[right]);
        quick_sort(records, left, i - 1, ascending);
        quick_sort(records, i + 1, right, ascending);
    }
    else {
        if (left >= right) return;

        int pivind = medianof3(records,left,right,ascending);
        swap (records[pivind],records[right]);
        double pivot = records[right].risk();

        int i = left;
        for (int j = left; j < right; j++) {
            if (records[j].risk() > pivot) {
                swap(records[i++], records[j]);
            }
        }
        swap(records[i], records[right]);
        quick_sort(records, left, i - 1, ascending);
        quick_sort(records, i + 1, right, ascending);
    }
}

//each function will use our sorting alg on each data structure and return the right value; // only male and female factor will also use merge sort.
void nth_ind_highest(int n, unordered_map<int, Bucket> &data,int factor) {
    bool ascending = false;
    cout << "These are the individuals at highest risk" << endl;
    cout << "factor: " << factors[factor-1] << endl<<endl;
    cout << "method: Quick sort"<<endl;
    int spacing = 10;


    vector<Person> temp = data[factor].PerVec;
    quick_sort(temp,0,temp.size()-1,ascending);

    int max = min<int>(n, temp.size());
    for (int i = 0; i < max; i++) {
        cout <<"ID: " <<left << setw(spacing)<< temp[i].id << "Risk value: " << temp[i].risk() <<endl;
    }

    if (factor == 1 || factor == 2){
        forward_list<Person> temp;
        temp = merge_sort(data[factor].PerList,ascending);
        cout <<endl;
        cout  <<"method: Merge sort" << endl;
        int count = 1;
        for (auto it : temp) {
            cout <<"ID: " <<left << setw(spacing) << it.id << "Risk value: " << it.risk() <<endl;
            if (count == n) {
                return;
            }
            count++;
        }
    }
}

void nth_ind_lowest(int n, unordered_map<int, Bucket> &data,int factor) {
    bool ascending = true;
    cout << "These are the individuals at lowest risk" << endl;
    cout << "factor: " << factors[factor-1] << endl<<endl;
    cout << "method: Quick sort"<<endl;
    int spacing = 10;

    vector<Person> temp = data[factor].PerVec;
    quick_sort(temp,0,temp.size()-1,ascending);


    int max = min<int>(n, temp.size());
    for (int i = 0; i < max; i++) {
        cout <<"ID: " <<left << setw(spacing)<< temp[i].id << "Risk value: " << temp[i].risk() <<endl;
    }


    if (factor == 1 || factor == 2){
        forward_list<Person> temp;
        temp = merge_sort(data[factor].PerList,ascending);
        cout <<endl;
        cout  <<"method: Merge sort" << endl;
        int count = 1;
        for (auto it : temp) {
            cout <<"ID: " <<left << setw(spacing) << it.id << "Risk value: " << it.risk() <<endl;
            if (count == n) {
                return;
            }
            count++;
        }
    }
}

int get_field() {
    string input;

    while (true) {
        cout <<endl<<"Enter the factor you would like to sort by: "<<endl;
        cout <<"male, female, smoker, non smoker,alcohol, no alcohol,family history, no family history, at risk, not at risk" <<endl<<endl;
        cout << "you can also type back to leave this command"<<endl;
        getline(cin, input);
        input = trimCopy(input);
        for (int i=0; i< input.length(); i++) {
            input[i] = tolower(input[i]);
        }
        if (input == "male") {
            return 1;
        }
        else if (input =="back") {
            return -1;
        }
        else if (input == "female") {
            return 2;
        }
        else if (input == "smoker") {
            return 3;
        }
        else if (input == "non smoker") {
            return 4;
        }
        else if (input == "alcohol") {
            return 5;
        }
        else if (input == "no alcohol") {
            return 6;
        }
        else if (input == "family history") {
            return 7;
        }
        else if (input == "no family history") {
            return 8;
        }
        else if (input == "at risk") {
            return 9;
        }
        else if (input == "not at risk") {
            return 10;
        }
        else {
            cout<< "invalid field" <<endl;
        }
    }
}
