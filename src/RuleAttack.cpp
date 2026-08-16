#include "../include/RuleAttack.hpp"
#include "../include/Hasher.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
#include <thread>
#include <mutex>

std::mutex ruleResultMutex;

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

static void ruleAttackWorker(
  const std::vector<std::string>& words,
  int start,
  int end,
  const std::string& hash,
  std::string (*hashFunction)(const std::string&),
  std::string& sharedResult,
  bool& found
){
  for(int i = start; i < end; i++){
    if(found){ return; }

    if(hashFunction(words[i]) == hash){
      std::lock_guard<std::mutex> lock(ruleResultMutex);
      sharedResult = words[i];
      found = true;
      return;
    }

    std::vector<std::string> mutations = mutationsManager(words[i]);
    for(const auto& w : mutations){
      if(found){ return; }

      if(hashFunction(w) == hash){
        std::lock_guard<std::mutex> lock(ruleResultMutex);
        sharedResult = w;
        found = true;
        return;
      }
    }
  }
}

std::string ruleAttackThreaded(
  const std::string& hash, 
  const std::string& algorithm, 
  const std::string& path,
  int numThreads
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

  std::vector<std::string> words;
  std::string line;
  while(getline(file, line)){
    words.push_back(line);
  }

  std::string sharedResult = "";
  bool found = false;

  int totalWords = words.size();
  int chunkSize = totalWords / numThreads;

  std::vector<std::thread> threads;

  for(int t = 0; t < numThreads; t++){
    int start = t * chunkSize;
    int end = (t == numThreads - 1) ? totalWords : start + chunkSize;

    threads.push_back(std::thread(
      ruleAttackWorker,
      std::cref(words), start, end, std::cref(hash), hashFunction,
      std::ref(sharedResult), std::ref(found)
    ));
  }

  for(auto& t : threads){
    t.join();
  }

  return sharedResult;
}