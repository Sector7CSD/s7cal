//
// Created by chris on 25.06.25.
//

#include "i18n.h"

void initLocalization(const std::string& applicationName, const std::string& dirname)
{
    setlocale(LC_ALL, "");
    bindtextdomain(applicationName.c_str(), dirname.c_str());
    textdomain(applicationName.c_str());
}