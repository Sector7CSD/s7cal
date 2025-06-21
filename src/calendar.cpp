#include "calendar.h"
#include "holidays.h"
#include "termcolor/termcolor.hpp"
#include "utils.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <utility>

Calendar::Calendar(int year, int month, bool threeMonthView, bool yearView, bool showWeekNumbers, Userconfig * pConfig)
    : year(year), month(month), threeMonthView(threeMonthView), yearView(yearView), showWeekNumbers(showWeekNumbers), pConfig(pConfig)
{
    holidays = getHolydays(year);
}

void Calendar::print3Months(std::vector<std::string> left, std::vector<std::string> center, std::vector<std::string> right)
{
    auto merged = Utils::mergeColumnsWithPadding({std::move(left), std::move(center), std::move(right)}, 2);
    for (const auto& line : merged)
    {
        std::cout << line << '\n';
    }
}

void Calendar::print()
{
    if (threeMonthView)
    {
        // calculate previous and next month (which could be in a different year)
        int prevMonth = month - 1, nextMonth = month + 1;
        int prevYear = year, nextYear = year;
        if (prevMonth < 1)
        {
            prevMonth = 12;
            prevYear--;
        }
        if (nextMonth > 12)
        {
            nextMonth = 1;
            nextYear++;
        }

        auto prevStream = printMonth(prevMonth, prevYear);
        auto curStream  = printMonth(month, year);
        auto nextStream = printMonth(nextMonth, nextYear);

        auto prevLines = Utils::streamToLines(prevStream);
        auto curLines  = Utils::streamToLines(curStream);
        auto nextLines = Utils::streamToLines(nextStream);

        print3Months(prevLines, curLines, nextLines);

    }
    else if(yearView)
    {
        std::vector<std::stringstream> months;
        for(int monthNumber = 1; monthNumber <= 12; monthNumber++)
        {
            months.push_back(printMonth(monthNumber, year));
        }

        for(int quater = 0; quater < 4; quater++)
        {
            print3Months(Utils::streamToLines(months[quater*3+0]),
                         Utils::streamToLines(months[quater*3+1]),
                         Utils::streamToLines(months[quater*3+2]));
            std::cout << std::endl;
        }

    }
    else
    {
        std::cout << printMonth(month, year).str();
    }
}

std::stringstream Calendar::printMonth(int aMonth, int aYear)
{
    static const std::string monthNamesDE[] = {"",
                                               "Januar",   "Februar",   "März",         // Q1
                                               "April",   "Mai",      "Juni",           // Q2
                                               "Juli",    "August",   "September",      // Q3
                                               "Oktober", "November", "Dezember"};    // Q4

    std::stringstream ss;
    ss<<termcolor::colorize;

    std::tm first_day = {};
    first_day.tm_mday = 1;
    first_day.tm_mon = aMonth - 1;
    first_day.tm_year = aYear - 1900;
    std::mktime(&first_day);

    time_t timestamp;
    time(&timestamp);
    std::tm now = {};
    now = *std::localtime(&timestamp);

    int lineLength = (7 * 3) - 1;
    lineLength += showWeekNumbers ? 3 : 0;

    auto title = std::string(monthNamesDE[aMonth]) + " " + std::to_string(aYear);
    auto titleLength = title.length();
    auto offset = (lineLength - titleLength) / 2;

    title.insert(0, offset, ' ');

    bool isCurrentMonth = now.tm_mon == first_day.tm_mon && now.tm_year == first_day.tm_year;

    if(isCurrentMonth)
    {
        ss << termcolor::cyan;
    }

    ss << title << termcolor::reset << std::endl;

    if (showWeekNumbers)
    {
        ss << "KW ";
    }

    ss << "Mo Di Mi Do Fr Sa So" << std::endl;

    int weekday = (first_day.tm_wday + 6) % 7; // Montag=0
    int days_in_month = 31;
    if (aMonth == 2)
    {
        days_in_month = 28 + ((aYear % 4 == 0 && aYear % 100 != 0) || (aYear % 400 == 0));
    }
    else if (aMonth == 4 || aMonth == 6 || aMonth == 9 || aMonth == 11)
    {
        days_in_month = 30;
    }

    int day = 1;
    while (day <= days_in_month)
    {
        std::tm date = first_day;
        date.tm_mday = day;
        date.tm_mon = aMonth - 1;
        date.tm_year = aYear - 1900;
        date.tm_hour = 12; // Workaround gegen Sommerzeitfehler
        std::mktime(&date);

        if (showWeekNumbers)
        {
            char buf[4];
            std::strftime(buf, sizeof(buf), "%V", &date);
            ss << termcolor::bold << buf << termcolor::reset << " ";
        }

        for (int i = 0; i < weekday; ++i)
        {
            ss << "   ";
        }

        for (; weekday < 7 && day <= days_in_month; ++weekday, ++day)
        {
            date.tm_mday = day;
            std::mktime(&date);

            bool resetNeeded = false;

            resetNeeded |= handleToday(ss, now, date);
            resetNeeded |= handleVacations(ss, date);
            resetNeeded |= handleBirthdays(ss, day, aMonth);
            resetNeeded |= handleWeekend(ss, date);
            resetNeeded |= handleHolidays(ss, day, aMonth);

            ss<< std::setw(2) << day;

            if (resetNeeded)
            {
                ss << termcolor::reset;
            }

            ss << " ";
        }
        weekday %= 7;
        ss << std::endl;
    }

    return ss;
}

bool Calendar::handleToday(std::stringstream & ss, std::tm & now, std::tm & date)
{
    bool isToday = isSameDay(&now, &date);
    if (isToday)
    {
        ss << termcolor::reverse;
    }

    return isToday;
}

bool Calendar::handleWeekend(std::stringstream & ss, std::tm date)
{
    bool isWeekend = date.tm_wday == 0 || date.tm_wday == 6;
    if(isWeekend)
    {
        ss << termcolor::red << termcolor::dark;
    }

    return isWeekend;
}

bool Calendar::handleHolidays(std::stringstream & ss, int aDay, int aMonth)
{
    bool isHoliday = holidays.count({aMonth, aDay}) > 0;
    if (isHoliday)
    {
        ss << termcolor::red << termcolor::bold;
    }

    return isHoliday;
}

bool Calendar::handleBirthdays(std::stringstream & ss, int aDay, int aMonth)
{
    if(pConfig != nullptr)
    {
        for(const auto & bday : pConfig->getBirthdays())
        {
            if (bday.day == aDay && bday.month == aMonth)
            {
                ss << termcolor::yellow << termcolor::italic;
                return true;
            }
        }
    }

    return false;
}

bool Calendar::handleVacations(std::stringstream & ss, std::tm date)
{
    bool isVacation = false;

    if(pConfig != nullptr)
    {
        for (auto& v : pConfig->getVacations())
        {
            std::tm current = date;
            time_t curr_time = std::mktime(&current);
            time_t from_time = std::mktime(&v.from);
            time_t to_time = std::mktime(&v.to);
            if (curr_time >= from_time && curr_time <= to_time)
            {
                isVacation = true;
                ss << termcolor::underline << termcolor::green;
                break;
            }
        }
    }

    return isVacation;
}

bool Calendar::isSameDay(std::tm *date1, std::tm *date2)
{
    return date1 && date2 &&
           date1->tm_mday == date2->tm_mday &&
           date1->tm_mon == date2->tm_mon &&
           date1->tm_year == date2->tm_year;
}
