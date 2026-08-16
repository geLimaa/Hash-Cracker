# HashCracker

HashCracker is a C++17 command-line tool for recovering plaintext values from hashes using three attack strategies:

- Dictionary attack
- Brute-force attack
- Rule-based mutation attack

The project supports threaded and sequential execution and uses OpenSSL for hash computation.

## Features

- Automatic hash type detection for `MD5`, `SHA1`, `SHA256`, and `SHA512`
- Interactive CLI with structured menus
- Dictionary attack with optional multithreading
- Brute-force attack with configurable charset and maximum length
- Rule-based attack with common word mutations
- CMake-based build system

## Requirements

- CMake 3.20 or newer
- A C++17-compatible compiler
- OpenSSL development libraries
- POSIX threads support

## Build

```bash
cmake -S . -B build
cmake --build build
```

The executable is generated at:

```bash
./build/hashcracker
```

## Usage

Run the binary and follow the prompts:

1. Enter the hash to crack
2. Enter the number of threads
3. Select the attack type
4. Provide the attack-specific parameters

### Supported attacks

#### Dictionary

Tries each word from a wordlist file until a match is found.

Required input:

- Path to the wordlist file

#### Brute force

Generates candidate strings from a selected charset up to a maximum length.

Available charsets:

- Lowercase letters
- Numbers
- Lowercase letters + numbers
- Full set with letters, numbers, and common symbols

Required input:

- Charset option
- Maximum length

#### Rule attack

Reads a wordlist and applies simple mutations such as:

- Capitalization
- Uppercasing
- Reversing
- Case toggling
- Digit suffixes
- Basic leetspeak substitutions

Required input:

- Path to the wordlist file

## Project Structure

```text
.
├── main.cpp
├── include/
│   ├── Cli.hpp
│   ├── DictionaryAttack.hpp
│   ├── BruteForceAttack.hpp
│   ├── Hasher.hpp
│   └── RuleAttack.hpp
├── src/
│   ├── Cli.cpp
│   ├── DictionaryAttack.cpp
│   ├── BruteForceAttack.cpp
│   ├── Hasher.cpp
│   └── RuleAttack.cpp
├── wordslist/
└── CMakeLists.txt
```

## Notes

- The program detects the hash algorithm from the hash length and format.
- Threaded execution is available for all attack modes.
- The tool is intended for educational and authorized use only.

