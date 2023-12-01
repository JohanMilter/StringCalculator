#pragma once

#include <iostream>
#include <queue>
#include <stack>
#include <regex>
#include <unordered_map>

struct regexPattern
{
	regexPattern(std::string regex) :
		pattern{ regex },
		reg{ regex }
	{
	};
	std::string pattern;
	std::regex reg;
};
struct regexMatch
{
	regexMatch(std::smatch m) : pos{ size_t(m.position()) }, value{ m.str() }
	{
	};
	regexMatch(std::string s) : value{ s }
	{
	};
	regexMatch()
	{
	};
	regexMatch(std::string s, std::size_t m) : pos{ m }, value{ s }
	{
	};

	std::size_t pos{};
	std::string value{};
};
using calcPointer = long double(*)(const long double&, const long double&);
using convertPointer = long double(*)(const std::string&, const std::size_t&);
using operatorInfo = std::tuple<int, std::string, regexPattern, calcPointer, convertPointer>;
using operatorMap = std::unordered_map<std::string, operatorInfo>;

extern operatorMap operators;
extern std::unordered_map<std::string, std::pair<int, bool>> neededSettings;
extern bool check_convertToPostfix;
extern bool check_evaluatePostfix;
extern std::string expr;

long double logb(long double x, long double b);
std::unordered_map<int, int> matchParentheses(std::string input, char left, char right);
bool matchRegex(std::string op);
int getPrecedence(std::string op);
calcPointer getOperatorMethod(std::string op);
std::vector<regexMatch> getRegexMatches(std::string text, std::regex regex);
bool isDecimal(std::string str);
operatorInfo getOperaInfo(const std::string op);
bool isSpecialOpera(const std::string op);
convertPointer getOperatorSpecial(std::string op);
std::queue<regexMatch> convertToPostfix(std::string& expression);
long double evaluatePostfix(std::queue<regexMatch> postfixQueue);
void prepareUserValues();
bool allLetters(const std::string& str);
bool hasNumbers(const std::string& str);
bool compareByFirst(const std::pair<const char*, long double>& a, const std::pair<const char*, long double>& b);
void replaceAll(std::string& originalString, const std::string& searchString, const std::string& replacementString);





