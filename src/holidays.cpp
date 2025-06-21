#include "holidays.h"
#include <ctime>
#include <tuple>

static std::pair<int, int> ostersonntag(int jahr)
{
    int k = jahr / 100;
    int m = 15 + ((3 * k + 3) / 4) - ((8 * k + 13) / 25);
    int s = 2 - ((3 * k + 3) / 4);
    int a = jahr % 19;
    int d = (19 * a + m) % 30;
    int r = (d + (a / 11)) / 29;
    int og = 21 + d - r;
    int sz = 7 - ((jahr + (jahr / 4) + s) % 7);
    int oe = 7 - ((og - sz) % 7);
    int ostertag = og + oe;

    if (ostertag > 31)
        return {4, ostertag - 31};
    return {3, ostertag};
}

std::set<std::pair<int, int>> getHolydays(int jahr)
{
    std::set<std::pair<int, int>> result;
    // feste Feiertage
    result.emplace(1, 1);   // Neujahr
    result.emplace(5, 1);   // Tag der Arbeit
    result.emplace(10, 3);  // Tag der dt. Einheit
    result.emplace(10, 31); // Reformationstag
    result.emplace(12, 25); // Weihnachten
    result.emplace(12, 26);

    // bewegliche Feiertage
    auto [om, od] = ostersonntag(jahr);
    std::tm o = {};
    o.tm_year = jahr - 1900;
    o.tm_mon = om - 1;
    o.tm_mday = od;
    std::mktime(&o);

    auto add_rel = [&](int offset) {
        std::tm temp = o;
        temp.tm_mday += offset;
        std::mktime(&temp);
        result.emplace(temp.tm_mon + 1, temp.tm_mday);
    };

    add_rel(-2); // Karfreitag
    add_rel(1);  // Ostermontag
    add_rel(39); // Christi Himmelfahrt
    add_rel(50); // Pfingstmontag

    return result;
}
