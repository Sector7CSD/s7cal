//
// Created by chris on 24.05.25.
//

#include "userconfig.h"
#include "data.h"

#include <filesystem>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "i18n.h"

void Userconfig::load()
{
    const std::string configPath = std::string(getenv("HOME")) + "/.config/s7cal/config.yml";
    if (!std::filesystem::exists(configPath))
    {
        return;
    }

    try
    {
        YAML::Node root = YAML::LoadFile(configPath);
        loadBirthdays(root);
        loadVacations(root);
    }
    catch (const YAML::Exception &e)
    {
        std::cerr << _("Error during parsing of the configuration.") << std::endl;
    }
}

std::tm Userconfig::parseDate(const std::string &dateStr, bool fix)
{
    std::tm t{};
    sscanf(dateStr.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    if(fix)
    {
        t.tm_hour = 12;
    }
    std::mktime(&t);
    return t;
}

void Userconfig::loadBirthdays(const YAML::Node &config)
{
    for (const auto &entry : config["birthdays"])
    {
        std::tm t = parseDate(entry["date"].as<std::string>(), false);
        birthdays.push_back({entry["name"].as<std::string>(), t.tm_mday, t.tm_mon + 1, t.tm_year + 1900});
    }
}

void Userconfig::loadVacations(const YAML::Node &config)
{
    for (const auto &entry : config["vacations"])
    {
        std::tm from = parseDate(entry["from"].as<std::string>(), false);
        std::tm to = parseDate(entry["to"].as<std::string>(), true);
        vacations.push_back({entry["name"].as<std::string>(), from, to});
    }
}

std::vector<Vacation> Userconfig::getVacations()
{
    return vacations;
}

std::vector<Birthday> Userconfig::getBirthdays()
{
    return birthdays;
}