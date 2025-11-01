#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include "main.h"
using namespace std;

vector<Person> people;
unordered_map<int, Bucket> dataMap;

unordered_map<int, Bucket> loadData(const string& path) {
    unordered_map<int, Bucket> data;
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file at " << path << endl;
        return data;
    }

    string line;
    getline(file, line); // skip header
    int id = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        Person p;
        p.id = id++;

        //can change w column order
        ss >> p.age >> p.bmi >> p.steps >> p.sleepHrs >> p.waterIntake
           >> p.caloriesConsumed >> p.smoker >> p.alcohol
           >> p.restingHeartRate >> p.systolicBP >> p.diastolicBP
           >> p.cholesterol >> p.famHistory >> p.atRisk;

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

void insertId(int id, unordered_map<int, Bucket>& data) {
}

void customizeId(int id, unordered_map<int, Bucket>& data) {
}

//display
void display(int id, unordered_map<int, Bucket>& data) {
    if (id < 0 || id >= (int)people.size()) {
        cout << "Invalid ID.\n";
        return;
    }
    const Person& p = people[id];
    cout << "ID: " << p.id << " | Age: " << p.age << " | BMI: " << p.bmi
         << " | Smoker: " << (p.smoker ? "Yes" : "No")
         << " | Alcohol: " << (p.alcohol ? "Yes" : "No")
         << " | Family History: " << (p.famHistory ? "Yes" : "No")
         << " | At Risk: " << (p.atRisk ? "Yes" : "No") << endl;
}

//sorting
void merge_sort(forward_list<Person> &list) {}
void quick_sort(vector<Person> &vec) {}

void nth_ind_highest(int n, unordered_map<int, Bucket>& data, int factor) {
}

void nth_ind_lowest(int n, unordered_map<int, Bucket>& data, int factor) {
}


int main(){
    string path;
    dataMap = loadData(path);

    return 0;
 }
