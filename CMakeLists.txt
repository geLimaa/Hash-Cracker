cmake_minimum_required(VERSION 3.20)
project(hashcracker CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

add_executable(hashcracker main.cpp src/Hasher.cpp src/DictionaryAttack.cpp src/BruteForceAttack.cpp src/RuleAttack.cpp)
target_include_directories(hashcracker PRIVATE src)

target_link_libraries(hashcracker PRIVATE OpenSSL::Crypto)