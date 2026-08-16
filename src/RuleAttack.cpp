#include "../include/RuleAttack.hpp"
#include "../include/Hasher.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>

static std::string capitalizeFirst(const std::string& word){
  if(word == ""){
    return "";
  }

  std::string capitalized = word;
  capitalized[0] = toupper(capitalized[0]);

  return capitalized;
}

static std::string uppercaseAll(const std::string& word){
  std::string uppercased = word;
  std::transform(uppercased.begin(), uppercased.end(), uppercased.begin(), ::toupper);

  return uppercased;
}

static void leetPermutations(
  const std::string& word,
  size_t index,
  std::string current,
  const std::map<char, char>& substitutions,
  std::vector<std::string>& results
){
  if(index == word.size()){
    results.push_back(current);
    return;
  }

  char c = word[index];
  auto it = substitutions.find(tolower(c));

  leetPermutations(word, index + 1, current + c, substitutions, results);

  if(it != substitutions.end()){
    leetPermutations(word, index + 1, current + it->second, substitutions, results);
  }
}

static std::vector<std::string> leetSubstitution(const std::string& word){
  std::map<char, char> substitutions = {
    {'a', '@'},
    {'e', '3'},
    {'i', '1'},
    {'o', '0'},
    {'s', '5'},
    {'t', '7'}
  };

  std::vector<std::string> results;
  leetPermutations(word, 0, "", substitutions, results);

  return results;
}

static std::vector<std::string> appendDigits(const std::string& word){
  std::vector<std::string> digitsAppended;

  for(int i = 0; i < 1000; i++){
    std::string number = std::to_string(i);
    std::string toAppend = word + number;
    digitsAppended.push_back(toAppend);
  }

  return digitsAppended;
}

static std::string reverse(const std::string& word){
  std::string reversed = word;
  std::reverse(reversed.begin(), reversed.end());

  return reversed;
}

static std::string toggleCase(const std::string& word){
  std::string toggled = word;

  for(auto& c : toggled){
    if(c >= 'a' && c <= 'z'){
      c = toupper(c);
    }
    else if(c >= 'A' && c <= 'Z'){
      c = tolower(c);
    }
    else{
      continue;
    }
  }

  return toggled;
}

static std::vector<std::string> mutationsManager(const std::string& word){
  std::vector<std::string> mutatedList;

  mutatedList.push_back(capitalizeFirst(word));
  mutatedList.push_back(uppercaseAll(word));
  mutatedList.push_back(reverse(word));
  mutatedList.push_back(toggleCase(word));

  std::vector<std::string> appendedDigitsList = appendDigits(word);
  for(auto w : appendedDigitsList){
    mutatedList.push_back(w);
  }

  std::vector<std::string> leetPermutationsList = leetSubstitution(word);
  for(auto w : leetPermutationsList){
    mutatedList.push_back(w);
  }

  return mutatedList;
}

std::string ruleAttack(
  const std::string& hash, 
  const std::string& algorithm, 
  const std::string& path
){
   
  std::string(*hashFunction)(const std::string&);

  if(algorithm == "md5"){
    hashFunction = md5;
  }
  else if(algorithm == "sha1"){
    hashFunction = sha1;
  }
  else if(algorithm == "sha256"){
    hashFunction = sha256;
  }
  else if(algorithm == "sha512"){
    hashFunction = sha512;
  }
  else{
    std::cerr << "Invalid Algorithm Option";
    return "";
  }

  std::ifstream file(path);
  if(!file.is_open()){
    std::cerr << "Error opening the file\n";
    return "";
  }

  std::string word;
  std::string answer;
  while(getline(file, word)){
    answer = hashFunction(word);
    if(answer == hash){
      answer = word;
      break;
    }
    else{
      std::vector<std::string> mutatedList = mutationsManager(word);

      for(auto w : mutatedList){
        answer = hashFunction(w);
        if(answer == hash){
          answer = w;
          return answer;
        }
      }
    }
    answer = "";
  }

  return answer;
}