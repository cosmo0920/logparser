#include <iostream>
#include <vector>
#include "log_level.hpp"
#include "java_log.hpp"
#include "profiler.hpp"
#include "summary.hpp"
#include "boost/iterator/transform_iterator.hpp"

using namespace std;

const int Size = 2048;

bool startsWith(const string str, const string& comp) {
    return equal(comp.begin(), comp.end(), str.begin());
}

bool startsWith(const char* str, const string& comp) {
    return equal(comp.begin(), comp.end(), str);
}

int main() {
    char line[Size];
    vector<string> lines;
    vector<unique_ptr<Profiler>> profilers;
    bool profile = false;
    while(cin.getline(line, Size)) {
        if(profile) {
            if(startsWith(line, "|--")) {
                lines.push_back(line);
            } else {
                auto prof = mkProfiler(lines.begin(), lines.end());
                profilers.push_back(prof);
                profile = false;
                lines.clear();
            }
        } else {
            string lineStr = string(line);
            auto log = mkJavaLog(lineStr);
            if(log != NULL && startsWith(log->message, "+ Profiler")) {
                profile = true;
            }
        }
    }
    auto totalF = [](Profiler p) { return p.total; };
    auto it = boost::make_transform_iterator(profilers.begin(), totalF);
    auto it_end = boost::make_transform_iterator(profilers.end(), totalF);
    auto totalSummary = mkSummary(it, it_end);
    cout << *totalSummary << endl;
    return EXIT_SUCCESS;
}
