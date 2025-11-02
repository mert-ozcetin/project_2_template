#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace sorting {

struct HealthRecord {
    int id = 0;
    int age = 0;
    std::string gender;
    double bmi = 0.0;
    int dailySteps = 0;
    double sleepHours = 0.0;
    double waterIntakeLiters = 0.0;
    int caloriesConsumed = 0;
    bool smoker = false;
    bool alcohol = false;
    int restingHeartRate = 0;
    int systolicBloodPressure = 0;
    int diastolicBloodPressure = 0;
    int cholesterol = 0;
    bool familyHistory = false;
    bool diseaseRisk = false;
};

enum class SortField {
    Id,
    Age,
    Gender,
    BodyMassIndex,
    DailySteps,
    SleepHours,
    WaterIntake,
    CaloriesConsumed,
    Smoker,
    Alcohol,
    RestingHeartRate,
    SystolicBloodPressure,
    DiastolicBloodPressure,
    Cholesterol,
    FamilyHistory,
    DiseaseRisk
};

using Comparator = std::function<bool(const HealthRecord&, const HealthRecord&)>;

SortField parseSortField(const std::string& fieldName);
Comparator makeComparator(SortField field, bool descending = true);
Comparator makeComparator(const std::string& fieldName, bool descending = true);

std::vector<HealthRecord> loadHealthRecords(const std::string& csvPath);

void mergeSort(std::vector<HealthRecord>& records, const Comparator& comparator);

std::vector<HealthRecord> mergeSortTopN(const std::vector<HealthRecord>& records, SortField field, bool descending = true, std::size_t topN = 50);
std::vector<HealthRecord> mergeSortTopN(const std::string& csvPath, SortField field, bool descending = true, std::size_t topN = 50);
std::vector<HealthRecord> mergeSortTopN(const std::string& csvPath, const std::string& field, bool descending = true, std::size_t topN = 50);

void printTopRecords(const std::vector<HealthRecord>& records, std::size_t topN = 50, std::ostream& os = std::cout);

}
