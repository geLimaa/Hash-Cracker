#include "../include/BruteForceAttack.hpp"
#include "../include/Hasher.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

std::mutex resultMutex;

static std::string bruteForce(
  const std::string& prefix, 
  int remaining, 
  const std::string& charset, 
  const std::string& hash, 
  std::string (*hashFunction)(const std::string&)
){
  if(remaining == 0){    
    if(hashFunction(prefix) == hash){
      return prefix;
    }
    else{
      return "";
    }
  }

  for(const auto c : charset){
    std::string result = bruteForce(prefix+c, remaining-1, charset, hash, hashFunction);
    if(result != ""){
      return result;
    }
  }

  return "";
}

static void bruteForceThreaded(
  const std::string& prefix, 
  int remaining, 
  const std::string& charset, 
  const std::string& hash, 
  std::string (*hashFunction)(const std::string&),
  std::string& sharedResult,
  bool& found
){
  if(found){ return; }

  if(remaining == 0){    
    if(hashFunction(prefix) == hash){
      std::lock_guard<std::mutex> lock(resultMutex);
      sharedResult = prefix;
      found = true;
    }
    return;
  }

  for(const auto c : charset){
    if(found) return;
    bruteForceThreaded(prefix + c, remaining - 1, charset, hash, hashFunction, sharedResult, found);
  }
}

std::string bruteForceAttack(
  const std::string& hash, 
  const std::string& algorithm, 
  const std::string& charset, 
  int maxLength
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

  for(int i = 1; i <= maxLength; i++){
    std::string result = bruteForce("", i, charset, hash, hashFunction);
    if(result != ""){
      return result;
    }
  }

  return "";
}

std::string bruteForceAttackThreaded(
  const std::string& hash, 
  const std::string& algorithm, 
  const std::string& charset, 
  int maxLength,
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

  std::string sharedResult = "";
  bool found = false;

  for(int len = 1; len <= maxLength; len++){
    std::vector<std::thread> threads;
    int charsetSize = charset.size();
    int chunkSize = charsetSize / numThreads;

    for(int t = 0; t < numThreads; t++){
      int start = t * chunkSize;
      int end = (t == numThreads - 1) ? charsetSize : start + chunkSize;
      std::string subCharset = charset.substr(start, end - start);

      threads.push_back(std::thread([=, &sharedResult, &found](){
        for(const auto c : subCharset){
          if(found){ return; }
            bruteForceThreaded(std::string(1, c), len - 1, charset, hash, hashFunction, sharedResult, found);
        }
      }));
    }

    for(auto& t : threads){
      t.join();
    }

    if(found){ return sharedResult; }
  }

  return "";
}