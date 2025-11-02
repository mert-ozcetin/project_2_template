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
