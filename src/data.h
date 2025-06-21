//
// Created by chris on 24.05.25.
//

#pragma once

#include <string>
#include <ctime>

struct Birthday
{
    std::string name;
    int day;
    int month;
    int year;
};

struct Vacation
{
    std::string name;
    std::tm from;
    std::tm to;
};