//
// Created by chris on 25.06.25.
//

#include "i18n.h"

void initLocalization() {
    setlocale(LC_ALL, "");
    bindtextdomain("s7cal", "/usr/share/locale");
    textdomain("s7cal");
}