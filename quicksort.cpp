#include "quicksort.h"
#include <algorithm>
#include <cstddef>

namespace sorting {
namespace {

std::ptrdiff_t partition(std::vector<HealthRecord>& records, std::ptrdiff_t low, std::ptrdiff_t high, const Comparator& comparator) {
    const HealthRecord pivot = records[static_cast<std::size_t>(high)];
    std::ptrdiff_t i = low - 1;
    for (std::ptrdiff_t j = low; j < high; ++j) {
        if (comparator(records[static_cast<std::size_t>(j)], pivot)) {
            ++i;
            std::swap(records[static_cast<std::size_t>(i)], records[static_cast<std::size_t>(j)]);
        }
    }
    std::swap(records[static_cast<std::size_t>(i + 1)], records[static_cast<std::size_t>(high)]);
    return i + 1;
}

void quickSortImpl(std::vector<HealthRecord>& records, std::ptrdiff_t low, std::ptrdiff_t high, const Comparator& comparator) {
    if (low >= high) {
        return;
    }
    const std::ptrdiff_t pivotIndex = partition(records, low, high, comparator);
    quickSortImpl(records, low, pivotIndex - 1, comparator);
    quickSortImpl(records, pivotIndex + 1, high, comparator);
}

}  

void quickSort(std::vector<HealthRecord>& records, const Comparator& comparator) {
    if (records.size() <= 1) {
        return;
    }
    quickSortImpl(records, 0, static_cast<std::ptrdiff_t>(records.size() - 1), comparator);
}

std::vector<HealthRecord> quickSortTopN(const std::vector<HealthRecord>& records, SortField field, bool descending, std::size_t topN) {
    std::vector<HealthRecord> sorted = records;
    quickSort(sorted, makeComparator(field, descending));
    if (sorted.size() > topN) {
        sorted.resize(topN);
    }
    return sorted;
}

std::vector<HealthRecord> quickSortTopN(const std::string& csvPath, SortField field, bool descending, std::size_t topN) {
    const auto records = loadHealthRecords(csvPath);
    return quickSortTopN(records, field, descending, topN);
}

std::vector<HealthRecord> quickSortTopN(const std::string& csvPath, const std::string& field, bool descending, std::size_t topN) {
    return quickSortTopN(csvPath, parseSortField(field), descending, topN);
}

}
