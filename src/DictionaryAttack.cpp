#include "../include/DictionaryAttack.hpp"
#include "../include/Hasher.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex dictResultMutex;

std::string dictionaryAttack(
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
    answer = "";
  }

  return answer;
}

static void dictionaryAttackWorker(
    const std::vector<std::string>& words,
    int start,
    int end,
    const std::string& hash,
    std::string (*hashFunction)(const std::string&),
    std::string& sharedResult,
    bool& found
){
    for(int i = start; i < end; i++){
        if(found) return;

        if(hashFunction(words[i]) == hash){
            std::lock_guard<std::mutex> lock(dictResultMutex);
            sharedResult = words[i];
            found = true;
            return;
        }
    }
}

std::string dictionaryAttackThreaded(
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
      dictionaryAttackWorker,
      std::cref(words), start, end, std::cref(hash), hashFunction,
      std::ref(sharedResult), std::ref(found)
    ));
  }

  for(auto& t : threads){
    t.join();
  }

  return sharedResult;
}