#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include "main.h"

using namespace std;

void displaycoms() {
    cout << endl << "The commands available are: "<< endl<<endl;
    cout << "insert: will insert a person and their medical information. You will first have to input a valid ,that is not already present, then all relevant information" <<endl<<endl;
    cout << "delete: will delete the person's record from the instance, not the database." <<endl<<endl;
    cout << "customize: will be similar to insert, but will also delete the already present id, if there is one." <<endl<<endl;
    cout << "nhighest: will print out the first nth at most risk individuals of a given factor."<<endl<<endl;
    cout << "nlowest: will print out the first nth at least risk individuals of a given factor."<<endl<<endl;
    cout << "display: will display the complete stats of the individual associated with an id." <<endl<<endl;
    cout << "help: will display the command options." <<endl<<endl;
    cout << "exit: will exit the program." <<endl;
    cout <<"at any point if you wish to go back/skip data input, you can input -1. "<<endl<<
           "This only works when being asked non boolean values"<<endl;
}
//int small doesnt test if its in the data, just if its a valid id
//type 1 is for id validation, type 2 will be for customize any other int will just be for a stat;
int validateintsmall(int type,unordered_map<int,Bucket> &data) {
    int temp =0 ;
    string input;
    while (true) {
        if (type ==1 || type ==2) {
            cout <<"Enter id: "<<endl;
        }
        else {
            cout<<"Enter n: "<<endl;
        }
        getline(cin, input);
        input = trimCopy(input);
        try{
            for (char c : input) {
                if ((c <'0' || c > '9')&&c!='-') {
                    throw -1;
                }
            }
            temp = stoi(input);
            if (type ==1) {
                if (temp>0) {
                    return temp;
                }
                else if (temp ==-1) {
                    return -1;
                }
                else {
                    cout << "Invalid input. Must be greater than 0" <<endl<<endl;
                }
            }
            else if (type == 2) {
                if (temp >= 0) {
                    if (temp == 0) {
                        cout << "Invalid input. Must be greater than 0" <<endl<<endl;
                        continue;
                    }
                    if (!checkid(temp, data)) {
                        return temp;
                    }
                    else {
                        cout << "ID not in database" <<endl<<endl;
                    }
                }
                else if (temp ==-1) {
                    return -1;
                }
                else {
                    cout << "Invalid input. Must be at least 0" <<endl<<endl;
                }
            }
            else {
                if (temp >= 0) {
                    return temp;
                }
                else if (temp ==-1) {
                    return -1;
                }
                else {
                    cout << "Invalid input. Must be at least 0" <<endl<<endl;
                }
            }
        }
        catch (out_of_range) {
            cout<<"Invalid input, argument out of range. Please try again." <<endl<<endl;
        }
        catch (...){
            cout << "Invalid input. Please try again." <<endl<<endl;
        }

    }
}

int validateint(string field,unordered_map<int,Bucket> data = {}) {
    int temp =0 ;
    string input;
    while (true) {
        cout <<"Enter " <<field <<": "<<endl;
        getline(cin, input);
        input = trimCopy(input);
        try{
            temp = stoi(input);
            for (char c : input) {
                if ((c <'0' || c > '9')&& c != '-') {
                    throw -1;
                }
            }
            if (temp >= 0) {
                if (field == "id") {
                    if (temp == 0) {
                        cout << "Invalid input. Must at greater than 0" <<endl<<endl;
                        continue;
                    }
                    if (checkid(temp,data)) {
                        return temp;
                    }
                    else {
                        cout<< "ID already in use" << endl<<endl;
                    }
                }
                else {
                    return temp;
                }
            }
            else if (temp == -1) {
                return -1;
            }
            else {
                cout << "Invalid input. Must at least 0" <<endl<<endl;
            }
        }
        catch (...){
            cout << "Invalid input. Please try again." <<endl<<endl;
        }
    }
}
double validatedouble(string field) {
    double temp =0 ;
    string input;

    while (true) {
        cout <<"Enter " <<field <<": "<<endl;
        getline(cin, input);
        input = trimCopy(input);
        try {
            for (char c : input) {
                if ((c <'0' || c > '9')&& c!= '.'&&c!='-') {
                    throw -1;
                }
            }
            temp = stod(input);
            if (temp >= 0) {
                return temp;
            }
            else if (temp == -1) {
                return -1;
            }
            else {
                cout << "Invalid input. Must at least 0" <<endl<<endl;
            }
        }
        catch (...) {
            cout << "Invalid input. Please try again." <<endl<<endl;
        }
    }
}
bool validatebool(string field) {
    string input;

    if (field == "gender") {
        while (true) {
            cout <<"Enter " <<field <<": "<<endl;
            getline(cin, input);
            input = trimCopy(input);
            for (int i= 0; i < input.length(); i++) {
                input[i] = tolower(input[i]);
            }
            if (input == "male") {
                return true;
            }
            else if (input =="female") {
                return false;
            }
            else {
                cout << "Invalid input." <<endl<<endl;
            }
        }
    }
    else if (field =="smoker" || field =="alcohol" || field =="family history"|| field =="atRisk") {
        while (true) {
            cout <<"Enter " <<field <<" (yes or no): "<<endl;
            getline(cin, input);
            input = trimCopy(input);
            for (int i= 0; i < input.length(); i++) {
                input[i] = tolower(input[i]);
            }
            if (input == "yes") {
                return true;
            }
            else if (input =="no") {
                return false;
            }
            else {
                cout << "Invalid input." <<endl<<endl;
            }
        }
    }
}

bool exitInt(int x) {
    if (x==-1){return true;};
    return false;
}
bool exitdouble(double x) {
    if (x==-1){return true;};
    return false;
}
bool exitperson(Person p) {
    if (exitInt(p.id)){return true;}
    if (exitInt(p.age)){return true;}
    if (exitInt(p.steps)){return true;}
    if (exitInt(p.caloriesConsumed)){return true;}
    if (exitInt(p.restingHeartRate)){return true ;}
    if (exitInt(p.systolicBP)){return true;}
    if (exitInt(p.diastolicBP)){return true;}
    if (exitInt(p.cholesterol)){return true;}
    if (exitdouble(p.sleepHrs)){return true;}
    if (exitdouble(p.waterIntake)){return true;}
    if (exitdouble(p.bmi)){return true;}
    return false;
}

void createperson(Person& p,unordered_map<int,Bucket> data,int type) {
    if (type == 1) {
        p.id = validateint("id",data);
    }
    else {
        p.id = validateintsmall(type,data);
    }
    if (exitInt(p.id)){return;}
    p.age = validateint("age");
    if (exitInt(p.age)){return;}
    p.steps = validateint("steps");
    if (exitInt(p.steps)){return;}
    p.caloriesConsumed = validateint("calories consumed");
    if (exitInt(p.caloriesConsumed)){return;}
    p.restingHeartRate = validateint("resting Heart Rate");
    if (exitInt(p.restingHeartRate)){return;}
    p.systolicBP = validateint("systolic BP");
    if (exitInt(p.systolicBP)){return;}
    p.diastolicBP = validateint("diastolic BP");
    if (exitInt(p.diastolicBP)){return;}
    p.cholesterol = validateint("cholesterol");
    if (exitInt(p.cholesterol)){return;}

    p.sleepHrs = validatedouble("sleep hours");
    if (exitdouble(p.sleepHrs)){return;}
    p.waterIntake = validatedouble("water intake");
    if (exitdouble(p.waterIntake)){return;}
    p.bmi = validatedouble("bmi");
    if (exitdouble(p.bmi)){return;}

    p.gender = validatebool("gender");
    p.smoker = validatebool("smoker");
    p.alcohol = validatebool("alcohol");
    p.famHistory = validatebool("family history");
    p.atRisk = validatebool("atRisk");
}

int main(){
    string path = "../resources/health_lifestyle_dataset.csv";
    bool exit = false;
    bool validinput = false;
    unordered_map<int, Bucket> data = loadData(path);

    // cout <<data[SMOKER].PerVec.size() << endl;
    // display(123, data);
    // deleteId(123,data);
    // display(123, data);
    // nth_ind_highest(5,data,2);

    cout << endl <<"Hello, this program will enable you to categorize an excel medical sheet, "
                   "and organize them based off ID and their formulated Risk. "
                   "This program will mainly use quicksort for the majority of the available factors to sort from, "
                   "but will also use mergesort for the male and female factors. To use a given command, "
                   "you must type in just the command and press enter, depending on the command, "
                   "you will be asked questions sequentially to narrow your fields." <<endl;

    displaycoms();

    while (!exit) {
        cout << endl <<"Please select an option: " <<endl;

        int id = -1;
        string input = "";
        validinput = false;
        getline (cin, input);
        input = trimCopy(input);
        for (int i= 0; i < input.length(); i++) {
            input[i] = tolower(input[i]);
        }
        if (input == "help") {
            displaycoms();
        }
        else if (input == "exit") {
            exit = true;
        }
        else if (input =="display") {
            id = validateintsmall(1,data);
            if (id != -1) {
                display(id,data);
            }
        }
        else if (input == "delete") {
            id = validateintsmall(1,data);
            if (id != -1) {
                deleteId(id,data);
            }
        }
        else if (input == "insert") {
            Person temp;
            createperson(temp,data,1);
            if (!exitperson(temp)) {
                insertPerson(temp,data);
            }
        }
        else if (input == "customize") {
            Person temp;
            createperson(temp,data,2);
            if (!exitperson(temp)) {
                customizeId(temp.id,data,temp);
            }
        }
        //id for these 2 are not id, just the nth term we will go up to.
        else if (input == "nhighest") {
            int field = get_field();
            if (field != -1) {
                id = validateintsmall(0,data);
                if (id != -1) {
                    nth_ind_highest(id,data,field);
                }
            };

        }
        else if (input == "nlowest") {
            int field = get_field();
            if (field != -1) {
                id = validateintsmall(0,data);
                if (id != -1) {
                    nth_ind_lowest(id,data,field);
                }
            }
        }
        else{
            cout << endl << "Invalid option, please try again." << endl;
        }
    }
 }