#pragma once
#include "userconfig.h"
#include <map>
#include <string>
#include <vector>

class Calendar
{
  public:

    Calendar(int year, int month, bool threeMonthView, bool yearView, bool showWeekNumbers, bool showAgenda, Userconfig * pConfig);
    void print();

  private:
    int year;
    int month;
    bool threeMonthView;
    bool yearView;
    bool showWeekNumbers;
    bool showAgenda;
    Userconfig * pConfig;

    std::map<std::pair<int, int>, std::string> holidays;

    std::vector<std::string> monthNames;

    [[nodiscard]] std::stringstream printMonth(int aMonth, int aYear) const;
    [[nodiscard]] std::stringstream printAgenda(int aMonth, int aYear);

    static bool isSameDay(const std::tm *date1, const std::tm *date2);
    static void print3Months(std::stringstream & ss, std::vector<std::string> left, std::vector<std::string> center, std::vector<std::string> right, size_t maxLineLength = 0);
    bool handleBirthdays(std::stringstream & ss, int aDay, int aMonth) const;
    bool handleHolidays(std::stringstream &ss, int aDay, int aMonth) const;
    static bool handleWeekend(std::stringstream &ss, const tm &date);
    static bool handleToday(std::stringstream &ss, const tm &now, const tm &date);
    bool handleVacations(std::stringstream &ss, const tm &date) const;

    static int daysInMonth(int year, int month) ;
};
