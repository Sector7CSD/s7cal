#include "holidays.h"
#include <ctime>
#include <tuple>
#include "i18n.h"

static std::pair<int, int> getEasterSunday(const int year)
{
    const int k = year / 100;
    const int m = 15 + (3 * k + 3) / 4 - (8 * k + 13) / 25;
    const int s = 2 - (3 * k + 3) / 4;
    const int a = year % 19;
    const int d = (19 * a + m) % 30;
    const int r = (d + a / 11) / 29;
    const int og = 21 + d - r;
    const int sz = 7 - (year + year / 4 + s) % 7;
    const int oe = 7 - (og - sz) % 7;
    const int easterSunday = og + oe;

    if (easterSunday > 31)
        return {4, easterSunday - 31};

    return {3, easterSunday};
}

std::map<std::pair<int, int>, std::string> getHolidays(const int year)
{
    std::map<std::pair<int, int>, std::string> result;

    // fixed holidays
    result[{1, 1}] = _("New Year");
    result[{5, 1}] = _("Labour Day");
    result[{10, 3}] = _("Day of German Unity");
    result[{10, 31}] = _("Reformation Day");
    result[{12, 25}] = _("Christmas Day");
    result[{12, 26}] = _("Boxing Day");

    // movable holidays
    auto [om, od] = getEasterSunday(year);
    std::tm o = {};
    o.tm_year = year - 1900;
    o.tm_mon = om - 1;
    o.tm_mday = od;
    std::mktime(&o);

    auto add_rel = [&](const int offset, const std::string& name) {
        std::tm temp = o;
        temp.tm_mday += offset;
        std::mktime(&temp);
        result[{temp.tm_mon + 1, temp.tm_mday}] = name;
    };

    add_rel(-2, _("Good Friday"));
    add_rel(1, _("Easter Monday"));
    add_rel(39, _("Ascension Day"));
    add_rel(50, _("Whit Monday"));

    return result;
}
