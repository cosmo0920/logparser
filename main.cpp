#include <iostream>
#include <vector>
#include "log_level.hpp"
#include "java_log.hpp"
#include "profiler.hpp"

using namespace std;

const int Size = 2048;

int main() {
    char line[Size];
    string prof = "+ Profiler";
    string stProf = "|--";
    vector<string> lines;
    bool profile = false;
    while(cin.getline(line, Size)) {
        if(profile) {
            if(equal(stProf.begin(), stProf.end(), line)) {
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
            if(log != NULL && equal(prof.begin(), prof.end(), log->message.begin())) {
                profile = true;
            }
        }
    }
}
