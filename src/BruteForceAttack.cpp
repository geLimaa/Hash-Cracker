#include "../include/BruteForceAttack.hpp"
#include "../include/Hasher.hpp"
#include <iostream>
#include <string>

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
