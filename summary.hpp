#pragma once

#include <iostream>
#include <cfloat>
#include <memory>
#include <algorithm>

class Summary {
public:
    const double sum;
    const double max;
    const double min;
    const int size;

    Summary(double _sum, double _max, double _min, int _size): sum(_sum), max(_max), min(_min), size(_size){};

    double average() const {
        return sum / size;
    }
};

template<class InputItr>
std::unique_ptr<Summary> mkSummary(InputItr first, InputItr last) {
    double sum = 0.0;
    double max = -DBL_MAX;
    double min = DBL_MAX;
    int size = 0;
    for(auto it = first; it != last; ++it) {
        double v = *it;
        sum += v;
        max = std::max(max, v);
        min = std::min(min, v);
        ++size;
    }
    return std::unique_ptr<Summary>(new Summary(sum, max, min, size));
}

std::ostream& operator<<(std::ostream& os, Summary& summary) {
    return os << "sum: " << summary.sum
              << ", max: " << summary.max
              << ", min: " << summary.min
              << ", average: " << summary.average();
}
