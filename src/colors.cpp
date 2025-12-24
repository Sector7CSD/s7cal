//
// Created by chris on 24.12.25.
//

#include "colors.h"

#include <termcolor/termcolor.hpp>

void colors::vacationsColor(std::ostream & ss)
{
    ss << termcolor::colorize << termcolor::underline << termcolor::green;
}

void colors::birthdaysColor(std::ostream & ss)
{
    ss << termcolor::colorize << termcolor::yellow << termcolor::italic;
}

void colors::todayColor(std::ostream & ss)
{
    ss << termcolor::reverse;
}

void colors::holidaysColor(std::ostream & ss)
{
    ss << termcolor::colorize << termcolor::bright_red << termcolor::bold;
}

void colors::weekendsColor(std::ostream & ss)
{
    ss << termcolor::colorize << termcolor::red << termcolor::dark;
}

void colors::currentMonthColor(std::ostream &ss)
{
    ss << termcolor::colorize << termcolor::cyan;
}

void colors::weekNumberColor(std::ostream & ss)
{
    ss << termcolor::bold;
}