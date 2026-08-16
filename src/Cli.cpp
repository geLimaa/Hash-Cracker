#include "../include/Cli.hpp"
#include "../include/Hasher.hpp"
#include "../include/DictionaryAttack.hpp"
#include "../include/BruteForceAttack.hpp"
#include "../include/RuleAttack.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>

const std::string CYAN = "\033[36m";
const std::string GREEN = "\033[32m";
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";

static void clearScreen(){
    std::cout << "\033[2J\033[H";
}

static void printBanner(){
    std::cout << CYAN << "========================================\n";
    std::cout << "  HASHCRACKER | v1.0.0\n";
    std::cout << "========================================" << RESET << "\n";
}

static void printMenu(){
    std::cout << "\nAvailable attacks:\n";
    std::cout << "  1) Dictionary\n";
    std::cout << "  2) Brute Force\n";
    std::cout << "  3) Rule Attack\n";
}

static std::string chooseCharset(){
    std::cout << "\nChoose a charset:\n";
    std::cout << "  1) Lowercase letters [a-z]\n";
    std::cout << "  2) Numbers [0-9]\n";
    std::cout << "  3) Lowercase + numbers\n";
    std::cout << "  4) Full set [lower + upper + numbers + symbols]\n";

    int choice;
    std::cout << "Charset option: ";
    std::cin >> choice;

    if(choice == 1) return "abcdefghijklmnopqrstuvwxyz";
    else if(choice == 2) return "0123456789";
    else if(choice == 3) return "abcdefghijklmnopqrstuvwxyz0123456789";
    else if(choice == 4) return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*";
    else return "";
}

static long long calculateTotalPossibilities(int charsetSize, int maxLength){
    long long total = 0;
    long long power = 1;
    for(int i = 1; i <= maxLength; i++){
        power *= charsetSize;
        total += power;
    }
    return total;
}

static void printProgress(long long current, long long total, double elapsedSeconds, bool firstPrint){
    double percent = (total > 0) ? (100.0 * current / total) : 0.0;
    double hashesPerSecond = (elapsedSeconds > 0) ? (current / elapsedSeconds) : 0.0;

    int barWidth = 30;
    int filled = (int)(barWidth * percent / 100.0);

    if(!firstPrint){
        std::cout << "\033[4A";
    }

    std::cout << CYAN << "[";
    for(int i = 0; i < barWidth; i++){
        if(i < filled) std::cout << "#";
        else std::cout << "-";
    }
    std::cout << "] " << RESET << BOLD << (int)percent << "%" << RESET << "          \n";

    std::cout << "Hashes tried:    " << current << " / " << total << "          \n";
    std::cout << "Speed:           " << GREEN << (long long)hashesPerSecond << " h/s" << RESET << "          \n";

    if(hashesPerSecond > 0){
        double remainingSeconds = (total - current) / hashesPerSecond;
        std::cout << "Estimated time:  " << (int)remainingSeconds << "s remaining          \n";
    }
    else{
        std::cout << "Estimated time:  calculating...          \n";
    }
}

static void printResult(const std::string& result, const std::string& hash, const std::string& algorithm, double seconds){
    if(result == ""){
        std::cout << "\nNo match found.\n";
        return;
    }

    std::cout << "\n" << GREEN << "CRACKED -----------------------------" << RESET << "\n";
    std::cout << BOLD << "Password: " << RESET << GREEN << result << RESET << "\n";
    std::cout << "Hash:      " << hash << "\n";
    std::cout << "Algorithm: " << algorithm << "\n";
    std::cout << "Time:      " << seconds << "s\n";
}

int runCli(){
    printBanner();

    std::string hash;
    std::cout << "Hash to break: ";
    std::cin >> hash;

    std::string algorithm = detectHashType(hash);
    if(algorithm == ""){
        std::cout << "Unsupported or invalid hash format.\n";
        return 1;
    }

    int numThreads;
    std::cout << "Number of threads (1 = sequential): ";
    std::cin >> numThreads;

    printMenu();
    int attackChoice;
    std::cout << "Attack option: ";
    std::cin >> attackChoice;

    std::string result;
    auto start = std::chrono::high_resolution_clock::now();

    if(attackChoice == 1){
        std::string path;
        std::cout << "Path to wordslist: ";
        std::cin >> path;

        if(numThreads > 1) result = dictionaryAttackThreaded(hash, algorithm, path, numThreads);
        else result = dictionaryAttack(hash, algorithm, path);
    }
    else if(attackChoice == 2){
        std::string charset = chooseCharset();
        if(charset == ""){
            std::cout << "Invalid charset option\n";
            return 1;
        }

        int maxLength;
        std::cout << "Max length: ";
        std::cin >> maxLength;

        long long totalPossibilities = calculateTotalPossibilities(charset.size(), maxLength);
        std::atomic<long long> hashCount(0);
        bool attackFinished = false;

        std::thread attackThread([&](){
            if(numThreads > 1){
                result = bruteForceAttackThreaded(hash, algorithm, charset, maxLength, numThreads, hashCount);
            }
            else{
                result = bruteForceAttack(hash, algorithm, charset, maxLength);
            }
            attackFinished = true;
        });

        clearScreen();
        printBanner();
        std::cout << "\nCracking...\n\n";

        bool firstPrint = true;
        while(!attackFinished){
            auto now = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double>(now - start).count();
            printProgress(hashCount.load(), totalPossibilities, elapsed, firstPrint);
            firstPrint = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        attackThread.join();
    }
    else if(attackChoice == 3){
        std::string path;
        std::cout << "Path to wordslist: ";
        std::cin >> path;

        if(numThreads > 1) result = ruleAttackThreaded(hash, algorithm, path, numThreads);
        else result = ruleAttack(hash, algorithm, path);
    }
    else{
        std::cout << "Invalid option\n";
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double totalSeconds = std::chrono::duration<double>(end - start).count();

    printResult(result, hash, algorithm, totalSeconds);

    return 0;
}