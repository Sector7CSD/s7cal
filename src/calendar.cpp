#include "calendar.h"
#include "holidays.h"
#include "termcolor/termcolor.hpp"
#include "colors.h"
#include "utils.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <utility>
#include "i18n.h"
#include "three_month_view.h"

Calendar::Calendar(const int year, const int month, const bool threeMonthView, const bool yearView, const bool showWeekNumbers, const bool showAgenda, Userconfig * pConfig)
    : year(year), month(month), threeMonthView(threeMonthView), yearView(yearView), showWeekNumbers(showWeekNumbers), showAgenda(showAgenda), pConfig(pConfig)
{
    holidays = getHolidays(year);

    monthNames = {
        "",
        _("January"),  _("February"), _("March"),       // Q1
        _("April"),    _("May"),      _("June"),        // Q2
        _("July"),     _("August"),   _("September"),   // Q3
        _("October"),  _("November"), _("December")     // Q4
    };
}

void Calendar::print3Months(std::stringstream & ss, std::vector<std::string> left, std::vector<std::string> center, std::vector<std::string> right, const size_t maxLineLength)
{
    const auto merged = Utils::mergeColumnsWithPadding({std::move(left), std::move(center), std::move(right)}, maxLineLength, 2);
    for (const auto& line : merged)
    {
        ss << line << '\n';
    }
}

void Calendar::print()
{
    std::stringstream calOut, agendaOut;

    size_t maxLineLength = 0;

    if (showAgenda)
    {
        if (threeMonthView)
        {
            size_t maxLines = 0;
            const auto listsOfLines = print3MonthView(month, year,[this](const int m, const int y)
            {
                return printAgenda(m, y);
            });

            Utils::getMaximumLineLength(listsOfLines, maxLineLength, maxLines);

            print3Months(agendaOut,listsOfLines[0], listsOfLines[1], listsOfLines[2]);
        }
        else if (yearView)
        {
            // FIXME: Think about how
        }
        else
        {
            agendaOut << printAgenda(month, year).str();
        }
    }

    if (threeMonthView)
    {
        const auto listsOfLines = print3MonthView(month, year,[this](const int m, const int y)
        {
            return printMonth(m, y);
        });

        print3Months(calOut, listsOfLines[0], listsOfLines[1], listsOfLines[2], maxLineLength);
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
            print3Months(calOut,
                           Utils::streamToLines(months[quater*3+0]),
                         Utils::streamToLines(months[quater*3+1]),
                         Utils::streamToLines(months[quater*3+2]));

            if (quater < 3)
            {
                calOut << std::endl;
            }
        }
    }
    else
    {
        calOut << printMonth(month, year).str();
    }

    std::cout << calOut.str() << std::endl;

    if (!agendaOut.str().empty())
    {
        std::cout << agendaOut.str() << std::endl;
    }
}

std::stringstream Calendar::printMonth(int aMonth, int aYear) const
{
    std::stringstream ss;
    ss<<termcolor::colorize;

    std::tm first_day = {};
    first_day.tm_mday = 1;
    first_day.tm_mon = aMonth - 1;
    first_day.tm_year = aYear - 1900;
    std::mktime(&first_day);

    time_t timestamp;
    time(&timestamp);
    std::tm now;
    now = *std::localtime(&timestamp);

    int lineLength = 7 * 3 - 1;
    lineLength += showWeekNumbers ? 3 : 0;

    auto title = std::string(monthNames[aMonth]) + " " + std::to_string(aYear);
    auto titleLength = title.length();
    auto offset = (lineLength - titleLength) / 2;

    title.insert(0, offset, ' ');

    // isCurrentMonth
    if(now.tm_mon == first_day.tm_mon && now.tm_year == first_day.tm_year)
    {
        colors::currentMonthColor(ss);
    }

    ss << title << termcolor::reset << std::endl;

    if (showWeekNumbers)
    {
        colors::weekNumberColor(ss);
        ss << _("CW") << termcolor::reset << " ";
    }

    ss << _("Mo Tu We Th Fr Sa Su") << std::endl;

    int weekday = (first_day.tm_wday + 6) % 7; // Montag=0

    int day = 1;
    while (day <= daysInMonth(aYear, aMonth))
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
            colors::weekNumberColor(ss);
            ss << buf << termcolor::reset << " ";
        }

        for (int i = 0; i < weekday; ++i)
        {
            ss << "   ";
        }

        for (; weekday < 7 && day <= daysInMonth(aYear, aMonth); ++weekday, ++day)
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

bool Calendar::handleToday(std::stringstream & ss, const std::tm & now, const std::tm & date)
{
    const bool isToday = isSameDay(&now, &date);
    if (isToday)
    {
        colors::todayColor(ss);
    }

    return isToday;
}

bool Calendar::handleWeekend(std::stringstream & ss, const std::tm &date)
{
    const bool isWeekend = date.tm_wday == 0 || date.tm_wday == 6;
    if(isWeekend)
    {
        colors::weekendsColor(ss);
    }

    return isWeekend;
}

bool Calendar::handleHolidays(std::stringstream & ss, int aDay, int aMonth) const
{
    const bool isHoliday = holidays.count({aMonth, aDay}) > 0;
    if (isHoliday)
    {
        colors::holidaysColor(ss);
    }

    return isHoliday;
}

std::stringstream Calendar::printAgenda(int aMonth, int aYear)
{
    std::map<int, std::vector<std::string>> agenda;

    // Holidays
    for (auto const& [date, name] : holidays)
    {
        if (date.first == aMonth)
        {
            std::stringstream mess;
            colors::holidaysColor(mess);
            mess << name << termcolor::reset;
            agenda[date.second].push_back(mess.str());
        }
    }

    // Birthdays
    if (pConfig != nullptr)
    {
        for (const auto& bday : pConfig->getBirthdays())
        {
            if (bday.month == aMonth)
            {
                std::stringstream mess;
                mess << _("Birthday of") << " ";
                colors::birthdaysColor(mess);
                mess << bday.name << termcolor::reset;
                agenda[bday.day].push_back(mess.str());
            }
        }

        // Vacations
        auto vacations = pConfig->getVacations();
        for (int day = 1; day <= daysInMonth(aYear, aMonth); ++day)
        {
            std::tm date = {};
            date.tm_mday = day;
            date.tm_mon = aMonth - 1;
            date.tm_year = aYear - 1900;
            date.tm_hour = 12;
            time_t curr_time = std::mktime(&date);

            for (auto [name, from, to] : vacations)
            {
                time_t from_time = std::mktime(&from);
                if (time_t to_time = std::mktime(&to); curr_time >= from_time && curr_time <= to_time)
                {
                    std::stringstream mess;
                    colors::vacationsColor(mess);
                    mess << name << termcolor::reset;
                    agenda[day].push_back(mess.str());
                }
            }
        }
    }

    std::stringstream ss;

    if (!agenda.empty())
    {
        for (auto const& [day, events] : agenda)
        {
            for (const auto& event : events)
            {
                ss << std::setw(2) << day << ". " << event << std::endl;
            }
        }
    }

    return ss;
}

int Calendar::daysInMonth(const int year, const int month)
{
    int days_in_month = 31;
    if (month == 2)
    {
        days_in_month = 28 + (year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        days_in_month = 30;
    }

    return days_in_month;
}

bool Calendar::handleBirthdays(std::stringstream & ss, const int aDay, const int aMonth) const
{
    if(pConfig != nullptr)
    {
        for(const auto & bday : pConfig->getBirthdays())
        {
            if (bday.day == aDay && bday.month == aMonth)
            {
                colors::birthdaysColor(ss);
                return true;
            }
        }
    }

    return false;
}

bool Calendar::handleVacations(std::stringstream & ss, const std::tm &date) const
{
    bool isVacation = false;

    if(pConfig != nullptr)
    {
        const auto vacations = pConfig->getVacations();
        for (auto v : vacations)
        {
            std::tm current = date;
            const time_t curr_time = std::mktime(&current);
            const time_t from_time = std::mktime(&v.from);
            if (const time_t to_time = std::mktime(&v.to); curr_time >= from_time && curr_time <= to_time)
            {
                isVacation = true;
                colors::vacationsColor(ss);
                break;
            }
        }
    }

    return isVacation;
}

bool Calendar::isSameDay(const std::tm *date1, const std::tm *date2)
{
    return date1 && date2 &&
           date1->tm_mday == date2->tm_mday &&
           date1->tm_mon == date2->tm_mon &&
           date1->tm_year == date2->tm_year;
}
