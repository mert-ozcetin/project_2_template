#pragma once
#include "mergesort.h"

namespace sorting {
void heapSort(std::vector<HealthRecord>& records, const Comparator& comparator);

std::vector<HealthRecord> heapSortTopN(const std::vector<HealthRecord>& records, SortField field, bool descending = true, std::size_t topN = 50);
std::vector<HealthRecord> heapSortTopN(const std::string& csvPath, SortField field, bool descending = true, std::size_t topN = 50);
std::vector<HealthRecord> heapSortTopN(const std::string& csvPath, const std::string& field, bool descending = true, std::size_t topN = 50);
} 
