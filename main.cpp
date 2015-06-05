#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <ctime>
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>
#include <iterator>

using namespace std;

const int Size = 2048;

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

const array<LogLevel, 6> logLevels{{
    LogLevel::Trace,
    LogLevel::Debug,
    LogLevel::Info,
    LogLevel::Warn,
    LogLevel::Error,
    LogLevel::Fatal}};

const array<string, 6> logLevelStrings{{
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"}};

LogLevel mkLogLevel(string str) {
    auto count = count_if(
        logLevelStrings.begin(),
        logLevelStrings.end(),
        [&str](string x){ return str == x; });
    return logLevels[count];
}

ostream& operator<<(ostream& os, LogLevel level) {
    auto count = count_if(
        logLevels.begin(),
        logLevels.end(),
        [&level](LogLevel l){ return l == level; });
    return os << logLevelStrings[count];
}

class JavaLog {
public:
    const tm& date;
    const LogLevel level;
    const string name;
    const string message;

    JavaLog(const tm& _date, const LogLevel& _level, const string& _name, const string& _message): date(_date), level(_level), name(_name), message(_message){};
};

unique_ptr<JavaLog> mkJavaLog(string& line) {
    const string dateStr = line.substr(0, 19);
    tm date;
    strptime(dateStr.c_str(), "%F %T", &date);
    auto begin = line.begin() + 26;
    auto end = find_if(begin, line.end(), [](char c){ return c == ']'; });
    if(end == line.end()) return NULL;
    string level = string(begin, end);
    begin = end + 3;
    end = find_if(begin, line.end(), [](char c){ return c == ']'; });
    if(end == line.end()) return NULL;
    string name = string(begin, end);
    string message = string(end + 2, line.end());
    return unique_ptr<JavaLog>(new JavaLog(date, mkLogLevel(level), name, message));
}

ostream& operator<<(ostream& os, JavaLog& log) {
    char date[30];
    strftime(date, 30, "%F %T", &log.date);
    return os << date << " [" << log.level << "] [" << log.name << "] " << log.message;
}

class Profiler {
public:
    const unordered_map<string, double> times;
    const double total;
    const string service;

    Profiler(unordered_map<string, double> _times, double _total, string _service): times(_times), total(_total), service(_service){};
};

template<class InputItr>
unique_ptr<Profiler> mkProfiler(InputItr begin, InputItr end) {
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

ostream& operator<<(ostream& os, Profiler& prof) {
    os << prof.service << endl;
    for(auto t: prof.times) {
        os << "\t" << t.first << ": " << t.second << "ms" << endl;
    }
    os << "Total: " << prof.total << "ms" << endl;
}

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
