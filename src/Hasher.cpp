#include "./Hasher.hpp"
#include <iomanip>
#include <sstream>
#include <map>
#include <cctype>

static std::string computeHash(const std::string& input, const EVP_MD* algorithm){
  EVP_MD_CTX* ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(ctx, algorithm, nullptr);
  EVP_DigestUpdate(ctx, input.c_str(), input.size());

  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int hashLen = 0;
  EVP_DigestFinal_ex(ctx, hash, &hashLen);
  EVP_MD_CTX_free(ctx);

  std::ostringstream oss;
  for(unsigned int i = 0; i < hashLen; i++){
    oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
  }

  return oss.str();
}

std::string md5(const std::string& input){ 
  return computeHash(input, EVP_md5()); 
}

std::string sha1(const std::string& input){ 
  return computeHash(input, EVP_sha1()); 
}

std::string sha256(const std::string& input){ 
  return computeHash(input, EVP_sha256()); 
}

std::string sha512(const std::string& input){ 
  return computeHash(input, EVP_sha512());
}

std::string detectHashType(const std::string& hash){
  std::map<int, std::string> algorithms{{32,"md5"},{40,"sha1"},{64,"sha256"},{128,"sha512"}};
  
  int size = hash.size();
  auto it = algorithms.find(size);

  if(it == algorithms.end()){
    return "";
  }

  for(char c : hash){
    c = tolower(c);
    if(!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))){
      return "";
    }
  }

  return it->second;
}
