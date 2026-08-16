#include "./include/Hasher.hpp"
#include "./include/DictionaryAttack.hpp"
#include "./include/BruteForceAttack.hpp"
#include "./include/RuleAttack.hpp"
#include <iostream>
#include <algorithm>
#include <string>

int main(){
  std::string hash;
  std::string algorithm;
  std::string typeAttack;

  std::cout << "Hash to break: ";
  std:: cin >> hash;
  algorithm = detectHashType(hash);

  int numThreads;
  std::cout << "Number of threads (1 = sequential): ";
  std::cin >> numThreads;

  std::cout << "Which type of attack [Dictionary/BruteForce/RuleAttack]: ";
  std::cin >> typeAttack;

  std::transform(typeAttack.begin(), typeAttack.end(), typeAttack.begin(), ::tolower);

  if(typeAttack == "dictionary"){
    std::string path;
    std::cout << "Path to wordslist: ";
    std::cin >> path;

    if(numThreads > 1){
      std::cout << dictionaryAttackThreaded(hash, algorithm, path, numThreads);
    }
    else{
      std::cout << dictionaryAttack(hash, algorithm, path);
    }
  }
  else if(typeAttack == "bruteforce"){
    std::cout << "Choose a charset:\n";
    std::cout << "1 - Lowercase (a-z)\n";
    std::cout << "2 - Numbers (0-9)\n";
    std::cout << "3 - Lowercase + numbers\n";
    std::cout << "4 - All characters (lower + upper + numbers + symbols)\n";

    int choice;
    std::cin >> choice;

    std::string charset;
    if(choice == 1){
        charset = "abcdefghijklmnopqrstuvwxyz";
    }
    else if(choice == 2){
        charset = "0123456789";
    }
    else if(choice == 3){
        charset = "abcdefghijklmnopqrstuvwxyz0123456789";
    }
    else if(choice == 4){
        charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    }
    else{
        std::cerr << "Invalid charset option\n";
        return 1;
    }

    int maxLength;
    std::cout << "Max length: ";
    std::cin >> maxLength;

    if(numThreads > 1){
      std::cout << bruteForceAttackThreaded(hash, algorithm, charset, maxLength, numThreads);
    }
    else{
      std::cout << bruteForceAttack(hash, algorithm, charset, maxLength);
    }  
  }
  else if(typeAttack == "ruleattack"){
    std::string path;
    std::cout << "Path to wordslist: ";
    std::cin >> path;

    if(numThreads > 1){
      std::cout << ruleAttackThreaded(hash, algorithm, path, numThreads);
    }
    else{
      std::cout << ruleAttack(hash, algorithm, path);
    }  
  }
  else{
    std::cout << "Invalid Option\n";
  }

  return 0;
}
