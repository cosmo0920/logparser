#pragma once

#include <memory>

class JavaLog {
public:
    const tm& date;
    const LogLevel level;
    const std::string name;
    const std::string message;

    JavaLog(const tm& _date, const LogLevel& _level, const std::string& _name, const std::string& _message): date(_date), level(_level), name(_name), message(_message){};
};

std::unique_ptr<JavaLog> mkJavaLog(std::string& line) {
    using namespace std;
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

std::ostream& operator<<(std::ostream& os, JavaLog& log) {
    char date[30];
    strftime(date, 30, "%F %T", &log.date);
    return os << date << " [" << log.level << "] [" << log.name << "] " << log.message;
}
