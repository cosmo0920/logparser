#include <iostream>
#include <vector>
#include "log_level.hpp"
#include "java_log.hpp"
#include "profiler.hpp"
#include "summary.hpp"

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
    bool profile = false;
    while(cin.getline(line, Size)) {
        if(profile) {
            if(startsWith(line, "|--")) {
                lines.push_back(line);
            } else {
                auto prof = mkProfiler(lines.begin(), lines.end());
                cout << *prof << endl;
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
    return EXIT_SUCCESS;
}
