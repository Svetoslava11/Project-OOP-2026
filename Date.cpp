#include "Date.h"
#include <ctime>
#include <iomanip>
#include <ostream>
#include <sstream>
namespace calorix {

    bool Date::isLeapYear(unsigned year) {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

    unsigned Date::daysInMonth(unsigned month, unsigned year) {
        static const unsigned days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        if (month == 2 && isLeapYear(year)) {
            return 29;
        }

        return days[month - 1];
    }

    bool Date::isValidDate(unsigned day, unsigned month, unsigned year) {
        if (year == 0 || month == 0 || month > 12 || day == 0) {
            return false;
        }

        return day <= daysInMonth(month, year);
    }

    Date::Date() : Date(Date::today()) {}

    Date::Date(unsigned day, unsigned month, unsigned year) : day(1), month(1), year(2026) {
        if (isValidDate(day, month, year)) {
            this->day = day;
            this->month = month;
            this->year = year;
        }
    }

    Date Date::today()
    {
        std::time_t now = std::time(nullptr);
        std::tm localTime{};

#ifdef _MSC_VER
        localtime_s(&localTime, &now);
#else
        localTime = *std::localtime(&now);
#endif

        return Date(
            static_cast<unsigned>(localTime.tm_mday),
            static_cast<unsigned>(localTime.tm_mon + 1),
            static_cast<unsigned>(localTime.tm_year + 1900)
        );
    }

    bool Date::parse(const std::string& text, Date& out) {
        std::istringstream iss(text);
        unsigned day = 0;
        unsigned month = 0;
        unsigned year = 0;
        char sep1 = '\0';
        char sep2 = '\0';

        if (!(iss >> day >> sep1 >> month >> sep2 >> year)) {
            return false;
        }

        if (!((sep1 == '.' || sep1 == '-') && (sep2 == '.' || sep2 == '-'))) {
            return false;
        }

        if (!isValidDate(day, month, year)) {
            return false;
        }

        out = Date(day, month, year);
        return true;
    }

    unsigned Date::getDay() const {
        return day;
    }

    unsigned Date::getMonth() const {
        return month;
    }

    unsigned Date::getYear() const {
        return year;
    }

    void Date::setDay(unsigned day) {
        if (isValidDate(day, month, year)) {
            this->day = day;
        }
    }

    void Date::setMonth(unsigned month) {
        if (isValidDate(day, month, year)) {
            this->month = month;
        }
    }

    void Date::setYear(unsigned year) {
        if (isValidDate(day, month, year)) {
            this->year = year;
        }
    }

    std::string Date::toString() const {
        std::ostringstream oss;
        oss << day << '.' << month << '.' << year;
        return oss.str();
    }

    void Date::print(std::ostream& os) const {
        os << toString();
    }

    bool Date::operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    bool Date::operator!=(const Date& other) const {
        return !(*this == other);
    }

}