#include "../include/DictionaryAttack.hpp"
#include "../include/Hasher.hpp"
#include <string>
#include <iostream>
#include <fstream>

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
