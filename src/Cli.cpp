#include "../include/Cli.hpp"
#include "../include/Hasher.hpp"
#include "../include/DictionaryAttack.hpp"
#include "../include/BruteForceAttack.hpp"
#include "../include/RuleAttack.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

namespace {

std::string readLine(const std::string& prompt) {
  std::string value;

  std::cout << prompt;
  std::getline(std::cin >> std::ws, value);

  return value;
}

int readInt(const std::string& prompt) {
  while (true) {
    std::string input = readLine(prompt);

    try {
      size_t processed = 0;
      int value = std::stoi(input, &processed);
      if (processed == input.size()) {
        return value;
      }
    } catch (...) {
    }

    std::cout << "Invalid number. Try again.\n";
  }
}

std::string chooseCharset() {
  std::cout << "\nChoose a charset:\n";
  std::cout << "  1) Lowercase letters [a-z]\n";
  std::cout << "  2) Numbers [0-9]\n";
  std::cout << "  3) Lowercase + numbers\n";
  std::cout << "  4) Full set [lower + upper + numbers + symbols]\n";

  switch (readInt("Charset option: ")) {
    case 1:
      return "abcdefghijklmnopqrstuvwxyz";
    case 2:
      return "0123456789";
    case 3:
      return "abcdefghijklmnopqrstuvwxyz0123456789";
    case 4:
      return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    default:
      return "";
  }
}

void printBanner() {
  std::cout << "========================================\n";
  std::cout << "             HASH CRACKER               \n";
  std::cout << "========================================\n";
}

void printMenu() {
  std::cout << "\nAvailable attacks:\n";
  std::cout << "  1) Dictionary\n";
  std::cout << "  2) Brute Force\n";
  std::cout << "  3) Rule Attack\n";
}

void printResult(const std::string& result) {
  if (result.empty()) {
    std::cout << "No match found.\n";
    return;
  }

  std::cout << "Result: " << result << '\n';
}

void runDictionary(const std::string& hash, const std::string& algorithm, int numThreads) {
  std::string path = readLine("Path to wordslist: ");
  std::string result;

  if (numThreads > 1) {
    result = dictionaryAttackThreaded(hash, algorithm, path, numThreads);
  } else {
    result = dictionaryAttack(hash, algorithm, path);
  }

  printResult(result);
}

void runBruteForce(const std::string& hash, const std::string& algorithm, int numThreads) {
  std::string charset = chooseCharset();
  if (charset.empty()) {
    std::cout << "Invalid charset option\n";
    return;
  }

  int maxLength = readInt("Max length: ");
  std::string result;

  if (numThreads > 1) {
    result = bruteForceAttackThreaded(hash, algorithm, charset, maxLength, numThreads);
  } else {
    result = bruteForceAttack(hash, algorithm, charset, maxLength);
  }

  printResult(result);
}

void runRuleAttack(const std::string& hash, const std::string& algorithm, int numThreads) {
  std::string path = readLine("Path to wordslist: ");
  std::string result;

  if (numThreads > 1) {
    result = ruleAttackThreaded(hash, algorithm, path, numThreads);
  } else {
    result = ruleAttack(hash, algorithm, path);
  }

  printResult(result);
}

}  

int runCli() {
  printBanner();

  std::string hash = readLine("Hash to break: ");
  std::string algorithm = detectHashType(hash);
  if (algorithm.empty()) {
    std::cout << "Unsupported or invalid hash format.\n";
    return 1;
  }

  int numThreads = readInt("Number of threads (1 = sequential): ");
  if (numThreads < 1) {
    std::cout << "Thread count must be at least 1.\n";
    return 1;
  }

  printMenu();
  int attackChoice = readInt("Attack option: ");

  std::cout << '\n';

  switch (attackChoice) {
    case 1:
      runDictionary(hash, algorithm, numThreads);
      break;
    case 2:
      runBruteForce(hash, algorithm, numThreads);
      break;
    case 3:
      runRuleAttack(hash, algorithm, numThreads);
      break;
    default:
      std::cout << "Invalid option\n";
      return 1;
  }

  return 0;
}
