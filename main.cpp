#include <iostream>
#include "./src/DictionaryAttack.hpp"
#include "./src/Hasher.hpp"

int main(){
  std::string hash;
  std::string algorithm;
  std::string path;

  std::cout << "Hash to break: ";
  std:: cin >> hash;
  algorithm = detectHashType(hash);

  std::cout << "Path to wordslist: ";
  std::cin >> path;

  std::cout << dictionaryAttack(hash, algorithm, path);
  return 0;
}