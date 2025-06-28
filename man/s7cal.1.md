% S7CAL(1)

# NAME

**s7cal** - display a calendar with colors in the terminal

# SYNOPSIS

| **s7cal** \[**OPTIONS**] \[**year**] \[**month**]

# DESCRIPTION

s7cal displays a simple calendar. If no arguments are specified, the current month is
displayed.

# OPTIONS

-h, \--help

:   Prints brief usage information.

-3, \--three
:   Show three month view (includes previous and next month)

-y, \--Year
:   Show the year view

-w, \--week
:   Show week numbers

# CONFIGURATION

In $HOME/.config/s7cal/config.yaml you can store birthdays and vacations. 
Birthdays will appear in italics and using yellow colors. 
Vacations will appear in underscore and using green color.
However, the colors will be overwritten by holidays and weekends, which are bold light red and dark red.

The configuration file should look like this:

```yaml
birthdays:
  - name: Hannes Wurst
    date: 1985-04-15
  - name: Hugo Haus
    date: 1979-07-23

vacations:
  - name: Easter
    from: 2025-04-14
    to: 2025-04-22
```

# BUGS

See GitHub Issues: <https://github.com/Sector7CSD/s7cal/issues>

# AUTHOR

Christian Klein <Sector7@gmx.de>
