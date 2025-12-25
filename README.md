# s7cal

`s7cal` is a lightweight, localized command-line calendar utility written in C++17. It provides a clean terminal interface for viewing months and years, with support for week numbers and agendas.

## Features

- **Month & Year Views**: Display a specific month or an entire year.
- **Three-Month View**: See the previous, current, and next month at a glance.
- **Week Numbers**: Optional display of ISO week numbers.
- **Agenda Support**: View upcoming events or holidays for the selected month.
- **Localization**: Fully localized interface (using `gettext`).
- **Customizable**: Configuration support via YAML.

## Usage

Run `s7cal` without arguments to see the current month.

```bash
s7cal [year] [month] [options]
```

## Options
- `year` The year to display (e.g., 2025).
- `month` The month to display (1-12).
- `-3, --three` Show the three-month view.
- `-y, --Year` Show the full year view.
- `-w, --week` Show week numbers.
- `-a, --agenda` Show agenda for the selected month.
- `-v, --version` Display version information.
- `-h, --help` Display help message.

## Building

### Prerequisites
- C++17 compatible compiler
- CMake 3.31+
- `yaml-cpp` library

### Build Instructions

```shell script
mkdir build && cd build
cmake ..
make
```

## Dependencies

The project automatically fetches the following dependencies via CMake:
- [CLI11](https://github.com/CLIUtils/CLI11): For command-line parsing.
- [termcolor](https://github.com/ikalnytskyi/termcolor): For cross-platform terminal colors.

## License

This project is licensed under the terms found in the `LICENSE` file.