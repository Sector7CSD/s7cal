//
// Created by chris on 24.12.25.
//

#ifndef S7CAL_THREE_MONTH_VIEW_H
#define S7CAL_THREE_MONTH_VIEW_H

#pragma once

#include "utils.h"

template <typename Printer>
std::vector<std::vector<std::string>> print3MonthView(int month, int year, Printer&& printer)
{
    int prevMonth = month - 1, nextMonth = month + 1;
    int prevYear = year, nextYear = year;

    if (prevMonth < 1) {
        prevMonth = 12;
        --prevYear;
    }
    if (nextMonth > 12) {
        nextMonth = 1;
        ++nextYear;
    }

    auto prevStream = printer(prevMonth, prevYear);
    auto curStream  = printer(month,     year);
    auto nextStream = printer(nextMonth,  nextYear);

    std::vector<std::vector<std::string>> sections = { Utils::streamToLines(prevStream), Utils::streamToLines(curStream), Utils::streamToLines(nextStream)};
    return sections;
}

#endif //S7CAL_THREE_MONTH_VIEW_H