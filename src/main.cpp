#include "calendar.h"
#include "userconfig.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include "i18n.h"

int main(int argc, char **argv)
{
    std::string appName = "s7cal";
    std::string versionString = appName + " v0.0.4";

    initLocalization(appName);

    CLI::App app{appName};
    app.set_version_flag("-v,--version", versionString);

    auto fmt = app.get_formatter();
    fmt->label("POSITIONALS", _("POSITIONALS"));
    fmt->label("OPTIONS", _("OPTIONS"));

    int month = 0;
    int year = 0;
    bool threeMonthView = false;
    bool yearView = false;
    bool showWeekNumbers = false;

    app.add_option(_("year"), year, _("year (e.g. 2025)"));
    app.add_option(_("month"), month, _("month (1-12)"));
    app.add_flag("-3,--three", threeMonthView, _("Show three month view (includes previous and next month)"));
    app.add_flag("-y,--Year", yearView, _("Show the year view"));
    app.add_flag("-w,--week", showWeekNumbers, _("Show week numbers"));

    CLI11_PARSE(app, argc, argv);

    if(month == 0 && year != 0)
    {
        yearView = true;
    }
    else
    {
        if (month == 0 || year == 0)
        {
            std::time_t t = std::time(nullptr);
            std::tm *now = std::localtime(&t);
            month = now->tm_mon + 1;
            year = now->tm_year + 1900;
        }
    }

    Userconfig config;
    config.load();

    Calendar calendar(year, month, threeMonthView, yearView, showWeekNumbers, &config);
    calendar.print();

    return 0;
}
