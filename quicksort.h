#pragma once
#include "mergesort.h"

namespace sorting {

void quickSort(std::vector<HealthRecord>& records, const Comparator& comparator);

std::vector<HealthRecord> quickSortTopN(const std::vector<HealthRecord>& records, SortField field, bool descending = true, std::size_t topN = 50);
std::vector<HealthRecord> quickSortTopN(const std::string& csvPath, SortField field, bool descending = true, std::size_t topN = 50);
std::vector<HealthRecord> quickSortTopN(const std::string& csvPath, const std::string& field, bool descending = true, std::size_t topN = 50);

}
