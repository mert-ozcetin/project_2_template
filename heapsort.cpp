#include "heapsort.h"
#include <algorithm>

namespace sorting {
void heapSort(std::vector<HealthRecord>& records, const Comparator& comparator) {
    if (records.empty()) {
        return;
    }

    auto heapComparator = [&comparator](const HealthRecord& lhs, const HealthRecord& rhs) {
        return comparator(rhs, lhs);
    };

    std::make_heap(records.begin(), records.end(), heapComparator);

    std::vector<HealthRecord> sorted;
    sorted.reserve(records.size());

    while (!records.empty()) {
        std::pop_heap(records.begin(), records.end(), heapComparator);
        sorted.push_back(records.back());
        records.pop_back();
    }

    records = std::move(sorted);
}


std::vector<HealthRecord> heapSortTopN(const std::vector<HealthRecord>& records, SortField field, bool descending, std::size_t topN) {
    std::vector<HealthRecord> sorted = records;
    heapSort(sorted, makeComparator(field, descending));
    if (sorted.size() > topN) {
        sorted.resize(topN);
    }
    return sorted;
}

std::vector<HealthRecord> heapSortTopN(const std::string& csvPath, SortField field, bool descending, std::size_t topN) {
    const auto records = loadHealthRecords(csvPath);
    return heapSortTopN(records, field, descending, topN);
}

std::vector<HealthRecord> heapSortTopN(const std::string& csvPath, const std::string& field, bool descending, std::size_t topN) {
    return heapSortTopN(csvPath, parseSortField(field), descending, topN);
}

}  
