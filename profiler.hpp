#pragma once

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <memory>

class Profiler {
public:
    const std::unordered_map<std::string, double> times;
    const double total;
    const std::string service;

    Profiler(std::unordered_map<std::string, double> _times, double _total, std::string _service): times(_times), total(_total), service(_service){};
};

template<class InputItr>
std::unique_ptr<Profiler> mkProfiler(InputItr begin, InputItr end) {
    using namespace std;
    stringstream ss;
    unordered_map<string, double> times;
    double total;
    string service;
    string temp;
    for(auto it = begin; it != end; ++it) {
        ss.clear();
        ss << *it;
        ss >> temp;
        string pattern; ss >> pattern;
        if(pattern == "elapsed") {
            ss >> temp;
            string _name; ss >> _name;
            string name = _name.substr(1, _name.length() - 2);
            double time; ss >> time;
            times[name] = time;
        } else if(pattern == "Total") {
            string name; ss >> name;
            service = name.substr(1, name.length() - 2);
            double time; ss >> time;
            total = time;
        } else {
            continue;
        }
    }
    return unique_ptr<Profiler>(new Profiler(times, total, service));
}

std::ostream& operator<<(std::ostream& os, Profiler& prof) {
    using namespace std;
    os << prof.service << endl;
    for(auto t: prof.times) {
        os << "\t" << t.first << ": " << t.second << "ms" << endl;
    }
    return os << "Total: " << prof.total << "ms" << endl;
}
