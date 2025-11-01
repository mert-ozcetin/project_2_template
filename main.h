#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <forward_list>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

struct Person {
    int id = 0;
    int age = 0;
    int bmi = 0;
    int steps = 0;
    int sleepHrs = 0;
    int waterIntake = 0;
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

    bool operator==(const Person& other) const {
        return id == other.id;
    }
};

struct Bucket {
    vector<Person> PerVec;
    forward_list<Person> PerList;
    vector<int> idsVec;
    list<int> idsList;
};

//load data , will handle the initial data processing and create the complete hash map
// the csv file is in resources
//1 = male
//2 = female

//3 = smoker
//4 = non smoker

//5 = alcohol
//6 = no alcohol

//7 = has fam history
//8 = doesn't have fam history

//9 = is at risk
//10 = is not at risk
unordered_map<int, Bucket> loadData(const string& path);

//first search through bucket 1 and 2 which are male and female, respeactively.
//determine which other buckets they are in and delete from data.
void deleteId(int id, unordered_map<int, Bucket> &data){}

// not fully sure how we are going to implement this since the info would have to somehow be passed in
// would also have to be hashed and inserted into each respective bucket
void insertId(int id, unordered_map<int, Bucket> &data){}

//similar to insert id with having to implement how we pass in the info as a param to the function.
//we just override the prexisting id's info.
void customizeId(int id, unordered_map<int, Bucket> &data){}

//generic display, we can just use male and female to find since they are guaranteed to be in one of them.
void display(int id, unordered_map<int, Bucket> &data){}

//our sorting algorithms - will sort based on risk factor not id.
void merge_sort(forward_list<Person> &list) {}
void quick_sort(vector<Person> &vec) {}

//each function will use our sorting alg on each data structure and return the right value;
void nth_ind_highest(int n, unordered_map<int, Bucket> &data,int factor){}
void nth_ind_lowest(int n, unordered_map<int, Bucket> &data,int factor){}
