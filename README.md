# ConfigSystem

`ConfigSystem` is a simple C++ project that reads text configuration in `key=value` format and stores the settings in typed `ConfigValue` objects.

## Main files

- `main.cpp`
  - example usage of `ConfigParser` and `Config`.
- `src/Config.h`
  - defines `ConfigValue` and `Config`.
  - `ConfigValue` uses `std::variant` to hold `string`, `int`, `bool`, `double`, or an embedded `Map` type.
- `src/Config.cpp`
  - implements configuration storage (`set`) and retrieval (`get`).
  - supports nested maps for dotted keys like `a.b.c`.
- `src/ConfigParser.h` / `src/ConfigParser.cpp`
  - parses configuration text and converts values to the appropriate type.
  - the `trim` helper removes unnecessary whitespace around keys and values.

## Current behavior

- `ConfigParser` splits the input into lines by `\n`.
- Each line is split into key and value by `=`.
- Surrounding whitespace is trimmed from both the key and the value.
- Value parsing uses the following type order:
  1. `int`
  2. `double`
  3. `bool`
  4. fallback to `std::string`

## Build and run

The project targets `C++20`.

Build with CMake:

```bash
cmake -B build -S .
cmake --build build
```

Run on Windows:

```bash
build\ConfigSystem.exe
```

## Notes

The `build/` directory contains CMake-generated files. This README documents the parser behavior and the main project components.
