#include "./BruteForceAttack.cpp"
#include "Hasher.cpp"
#include <iostream>

static std::string bruteForce(
  const std::string& prefix, 
  int remaining, 
  const std::string& charset, 
  const std::string& hash, 
  std::string (*hashFunction)(const std::string&)
){

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

  return bruteForce("", maxLength, charset, hash, hashFunction);
}