//
// Created by chris on 24.12.25.
//

#pragma once
#include <iosfwd>

class colors
{
public:
    static void vacationsColor(std::ostream & ss);
    static void birthdaysColor(std::ostream & ss);
    static void todayColor(std::ostream & ss);
    static void holidaysColor(std::ostream & ss);
    static void weekendsColor(std::ostream & ss);
    static void currentMonthColor(std::ostream & ss);
    static void weekNumberColor(std::ostream & ss);
};
