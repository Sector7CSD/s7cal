//
// Created by chris on 24.05.25.
//

#include "utils.h"

std::string Utils::stripAnsiEscapeCodes(const std::string &input)
{
    static const std::regex ansi_regex(R"(\x1B\[[0-9;]*[A-Za-z])");
    return std::regex_replace(input, ansi_regex, "");
}

void Utils::getMaximumLineLength(const std::vector<std::vector<std::string>> &columns, size_t & maxLineLength, size_t & maxLines)
{
    for (const auto &col : columns)
    {
        if (col.size() > maxLines)
            maxLines = col.size();

        for(const auto& line : col)
        {
            auto undecoratedLine = stripAnsiEscapeCodes(line);
            if(const size_t lineLength = undecoratedLine.length(); lineLength > maxLineLength)
            {
                maxLineLength = lineLength;
            }
        }
    }
}

std::vector<std::string> Utils::mergeColumnsWithPadding(const std::vector<std::vector<std::string>> &columns,
                                                        size_t maxLineLength, const int spacing)
{
    std::vector<std::string> result;

    // Calculate the maximum of all rows from all columns
    size_t maxLines = 0;
    getMaximumLineLength(columns, maxLineLength, maxLines);

    // Padding between the columns
    const std::string pad(spacing, ' ');

    // Merge rows together
    for (size_t i = 0; i < maxLines; ++i)
    {
        std::string line;
        for (size_t col = 0; col < columns.size(); ++col)
        {
            std::string part;
            if (i < columns[col].size())
                part = columns[col][i];

            // Calculate visible length
            std::string visible = stripAnsiEscapeCodes(part);
            const size_t visible_length = visible.length();

            // Fill in on the right so that everything is flush
            if (const size_t col_width = maxLineLength; visible_length < col_width)
                part += std::string(col_width - visible_length, ' ');

            line += part;

            if (col < columns.size() - 1)
                line += pad;
        }
        result.push_back(line);
    }

    return result;
}

std::vector<std::string> Utils::streamToLines(const std::stringstream& ss)
{
    std::vector<std::string> lines;
    std::string line;
    std::istringstream input(ss.str());
    while (std::getline(input, line))
    {
        lines.push_back(line);
    }
    return lines;
}
