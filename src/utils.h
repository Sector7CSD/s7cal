//
// Created by chris on 24.05.25.
//

#pragma once

#include <regex>
#include <string>
#include <vector>

class Utils
{
  public:
    static std::string              stripAnsiEscapeCodes(const std::string &input);
    static std::vector<std::string> mergeColumnsWithPadding(const std::vector<std::vector<std::string>> &columns,
                                                            size_t maxLineLength,
                                                            int spacing = 2);

    static void                     getMaximumLineLength(const std::vector<std::vector<std::string>> &columns, size_t & maxLineLength, size_t & maxLines);

    static std::vector<std::string> streamToLines(const std::stringstream &ss);
};
