#include <iostream>
#include "Hasher.hpp"

int main(){
  // std::string input = "password";

  // std::cout << "MD5 of 'password': " << md5(input) << "\n";
  // std::cout << "SHA1 of 'password': " << sha1(input) << "\n";
  // std::cout << "SHA256 of 'password': " << sha256(input) << "\n";
  // std::cout << "SHA512 of 'password': " << sha512(input) << "\n";

  std::string hash; 
  std::cin >> hash;

  std::cout << detectHashType(hash);

  return 0;
}