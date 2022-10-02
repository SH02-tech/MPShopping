/**
 * @file DateTime.cpp
 * @author DECSAI
 * @note To be implemented by students 
 * 
 */

#include <iostream>
#include <sstream>
#include "DateTime.h"
using namespace std;

/**
 * @brief auxiliary function to check if the date and the time are in correct 
 * format, taking into account, leap year etc.
 * @param year input
 * @param month input
 * @param day input
 * @param hour input
 * @param min input
 * @param sec input
 * @return true if is correct, false if not
 */
bool isCorrect(int year, int month, int day, int hour, int min, int sec);
/**
 * @brief split the first field in 6 components of the data time.
 * Please consider using string::substr(int, int)[https://en.cppreference.com/w/cpp/string/basic_string/substr] to cut the line
 * into the appropriate substrings and then convert it into integer values with 
 * the function stoi(string) [https://en.cppreference.com/w/cpp/string/basic_string/stol]
 * ~~~~
 *      YYYY-MM-dd hh:mm:ss UTC 
 *      +----+----+----+----+----+ 
 *      |    |    |    |    |    |
 *      0    5    10   15   20   25
 * ~~~~
 *  * @param line input string
 * @param y output int
 * @param m output int
 * @param d output int
 * @param h output int
 * @param mn output int
 * @param s output int
 */
void split(const std::string &line, int &y, int &m, int &d, int &h, int &mn, int &s);

DateTime::DateTime() {
    initDefault();
}

void DateTime::initDefault() {
    split(DATETIME_DEFAULT, _year, _month, _day, _hour, _min, _sec);
}

bool isCorrect(int year, int month, int day, int hour, int min, int sec) {
    bool correct = false;
    
    const int MAX_YEAR = 9999;
    const int MAX_MONTH = 12;
    const int MAX_HOUR = 23;
    const int MAX_MIN = 59;
    const int MAX_SEC = 59;
    
    int days_month[MAX_MONTH] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    // Leap year considered.
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        days_month[1]++;
    
    if (1000 <= year && year <= MAX_YEAR)
        if (1 <= month && month <= MAX_MONTH)
            if (1 <= day && day <= days_month[month-1])
                if (0 <= hour && hour <= MAX_HOUR)
                    if (0 <= min && min <= MAX_MIN)
                        if (0 <= sec && sec <= MAX_SEC)
                            correct = true;
    
    return correct;
}

void split(const std::string &line, int &y, int &m, int &d, int &h, int &mn, int &s) {
    try {
        y = stoi(line.substr(0,4));
        m = stoi(line.substr(5,2));
        d = stoi(line.substr(8,2));
        h = stoi(line.substr(11,2));
        mn = stoi(line.substr(14,2));
        s = stoi(line.substr(17,2));
        
        if (!isCorrect(y, m, d, h, mn, s))
            split(DATETIME_DEFAULT, y, m, d, h, mn, s);
        
    } catch (const std::invalid_argument &e) {
        cout << "An exception happened." << endl;
        split(DATETIME_DEFAULT, y, m, d, h, mn, s);
    }
    
}

void DateTime::set(const std::string &line) {
    split(line, _year, _month, _day, _hour, _min, _sec);
}

DateTime::DateTime(const string &line) {
    split(line, _year, _month, _day, _hour, _min, _sec);
}

int DateTime::year() const {
    return _year;
}

int DateTime::month() const {
    return _month;
}

int DateTime::day() const {
    return _day;
}

int DateTime::hour() const {
    return _hour;
}

int DateTime::min() const {
    return _min;
}

int DateTime::sec() const {
    return _sec;
}

bool DateTime::isBefore(const DateTime &one) const {
    
    // As the DateTime is ordered as yyyy-mm-dd-hh-mm-ss, we may just compare
    // both dates to find which is before (alphabetically and numerically).
    
    return (to_string().compare(one.to_string()) < 0 ? true : false);
}

bool DateTime::sameDay(const DateTime &other) const {
    return (_year == other._year && _month == other._month && _day == other._day ? true : false);
}

int DateTime::weekDay() const {
    int a = (14 - _month) / 12;
    int y = _year - a;
    int m = _month + 12*a - 2;

    return ((_day + y + y/4 - y/100 + y/400 + 31*m/12) % 7);
}

string DateTime::to_string() const {
    char local[64];
    sprintf(local, "%04i-%02i-%02i %02i:%02i:%02i UTC", _year, _month, _day, _hour, _min, _sec);
    return local;
}

