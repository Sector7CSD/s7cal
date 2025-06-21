#pragma once
#include "userconfig.h"
#include <ctime>
#include <map>
#include <set>
#include <string>
#include <vector>

class Calendar
{
  public:

    Calendar(int year, int month, bool threeMonthView, bool yearView, bool showWeekNumbers, Userconfig * pConfig);
    void print();

  private:
    int year;
    int month;
    bool threeMonthView;
    bool yearView;
    bool showWeekNumbers;
    Userconfig * pConfig;

    std::set<std::pair<int, int>> holidays;

    std::stringstream printMonth(int aMonth, int aYear);

    static bool isSameDay(std::tm *date1, std::tm *date2);
    static void print3Months(std::vector<std::string> left, std::vector<std::string> center, std::vector<std::string> right);
    bool handleBirthdays(std::stringstream & ss, int aDay, int aMonth);
    bool handleHolidays(std::stringstream &ss, int aDay, int aMonth);
    static bool handleWeekend(std::stringstream &ss, tm date);
    static bool handleToday(std::stringstream &ss, tm &now, tm &date);
    bool handleVacations(std::stringstream &ss, tm date);
};
