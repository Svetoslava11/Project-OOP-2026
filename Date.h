#pragma once
#include<iosfwd>
#include <string>
namespace calorix {
	class Date
	{
	private: unsigned day;
		   unsigned month;
		   unsigned year;
		   static bool isLeapYear(unsigned year);
		   static unsigned daysInMonth(unsigned mmonth, unsigned year);
		   static bool isValidDate(unsigned day, unsigned month, unsigned year);
	public:
		Date();
		Date(unsigned day, unsigned month, unsigned year);

		static bool parse(const std::string& text, Date& out);
		static Date today();

		unsigned getDay()const;
		unsigned getMonth() const;
		unsigned getYear()const;

		void setDay(unsigned day);
		void setMonth(unsigned month);
		void setYear(unsigned year);

		std::string toString()const;
		void print(std::ostream& os)const;

		bool operator==(const Date& other)const;
		bool operator!=(const Date& other)const;
	};
}

