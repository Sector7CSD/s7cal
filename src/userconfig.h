//
// Created by chris on 24.05.25.
//

#pragma once

#include "data.h"
#include <string>
#include <vector>
#include <yaml-cpp/node/node.h>

class Userconfig
{
  public:

    void load();

    std::vector<Vacation> getVacations();
    std::vector<Birthday> getBirthdays();

  protected:

    std::vector<Vacation> vacations;
    std::vector<Birthday> birthdays;

    tm parseDate(const std::string &dateStr, bool fix);
    void loadVacations(const YAML::Node &config);
    void loadBirthdays(const YAML::Node &config);

};
