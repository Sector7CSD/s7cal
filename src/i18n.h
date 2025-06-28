//
// Created by chris on 25.06.25.
//

#ifndef S7CAL_I18N_H
#define S7CAL_I18N_H

#include <libintl.h>
#include <clocale>
#include <string>

void initLocalization(const std::string& applicationName, const std::string& dirname = "/usr/share/locale");

#define _(STRING) gettext(STRING)
#define N_(STRING) (STRING)
#define ngettext_expr(SING, PLUR, N) ngettext(SING, PLUR, N)

#endif // S7CAL_I18N_H
