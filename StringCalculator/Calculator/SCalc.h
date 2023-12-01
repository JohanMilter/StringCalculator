#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>

class SCalc
{
public:
    // Declare public methods
    static long double calculate(std::string expr);
    static std::string giveValues(std::string expression, std::vector<std::pair<const char*, long double>> values);
    static std::unordered_map<std::string, std::pair<int, bool>> settings;
    static std::vector<std::pair<std::string, std::string>> calcSteps;
    static std::vector<std::pair<const char*, long double>> values; 
private:
    
};

