#include "holidays.h"
#include <ctime>
#include <tuple>
#include "i18n.h"

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

std::map<std::pair<int, int>, std::string> getHolydays(int jahr)
{
    std::map<std::pair<int, int>, std::string> result;
    // feste Feiertage
    result[{1, 1}] = _("New Year");
    result[{5, 1}] = _("Labour Day");
    result[{10, 3}] = _("Day of German Unity");
    result[{10, 31}] = _("Reformation Day");
    result[{12, 25}] = _("Christmas Day");
    result[{12, 26}] = _("Boxing Day");

    // bewegliche Feiertage
    auto [om, od] = ostersonntag(jahr);
    std::tm o = {};
    o.tm_year = jahr - 1900;
    o.tm_mon = om - 1;
    o.tm_mday = od;
    std::mktime(&o);

    auto add_rel = [&](int offset, const std::string& name) {
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
