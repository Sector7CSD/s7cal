#include "calendar.h"
#include "userconfig.h"
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    CLI::App app{"s7cal"};

    int month = 0;
    int year = 0;
    bool threeMonthView = false;
    bool yearView = false;
    bool showWeekNumbers = false;

    app.add_option("year", year, "year (z.B. 2025)");
    app.add_option("month", month, "month (1-12)");
    app.add_flag("-3", threeMonthView, "Show three month view (includes previous and next month");
    app.add_flag("-y", yearView, "Show the year view");
    app.add_flag("-w", showWeekNumbers, "Show week numbers");

    CLI11_PARSE(app, argc, argv);

    if (month == 0 || year == 0)
    {
        std::time_t t = std::time(nullptr);
        std::tm *now = std::localtime(&t);
        month = now->tm_mon + 1;
        year = now->tm_year + 1900;
    }

    Userconfig config;
    config.load();

    Calendar calendar(year, month, threeMonthView, yearView, showWeekNumbers, &config);
    calendar.print();

    return 0;
}
