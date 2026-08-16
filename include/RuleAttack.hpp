#pragma once 

#include <string>
#include <vector>
#include <map>

static std::string capitalizeFirst(const std::string& word);
static std::string uppercaseAll(const std::string& word);
static std::vector<std::string> appendDigits(const std::string& word);
static std::string reverse(const std::string& word);
static std::string toggleCase(const std::string& word);

static std::vector<std::string> leetSubstitution(const std::string& word);
static void leetPermutations(const std::string& word, size_t index, std::string current, const std::map<char, char>& substitutions, std::vector<std::string>& results);

static std::vector<std::string> mutationsManager(const std::string& word);

std::string ruleAttack(const std::string& hash, const std::string& algorithm, const std::string& path);

static void ruleAttackWorker(const std::vector<std::string>& words, int start, int end, const std::string& hash, std::string (*hashFunction)(const std::string&), std::string& sharedResult, bool& found);

std::string ruleAttackThreaded(const std::string& hash, const std::string& algorithm, const std::string& path, int numThreads);