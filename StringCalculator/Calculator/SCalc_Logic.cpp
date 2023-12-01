#include "SCalc_Logic.h"
#include "SCalc.h"
#include <chrono>


std::string expr;
std::vector<std::pair<std::string, std::string>> SCalc::calcSteps;
std::vector<std::pair<const char*, long double>> SCalc::values;
std::unordered_map<std::string, std::pair<int, bool>> SCalc::settings;

long double SCalc::calculate(std::string expre)
{
	if (expre == "" || allLetters(expre) || !hasNumbers(expre))
		return NAN;
	calcSteps.clear();
	prepareUserValues();
	return evaluatePostfix(convertToPostfix(expre));
}
std::string SCalc::giveValues(std::string expression, std::vector<std::pair<const char*, long double>> values)
{
	std::sort(values.begin(), values.end(), compareByFirst);
	for (const std::pair<const char*, long double>& v : values)
		replaceAll(expression, v.first, std::to_string(v.second));
	return expression;
}
std::unordered_map<int, int> matchParentheses(std::string input, char left, char right)
{
	std::unordered_map<int, int> matchingPairs;
	std::stack<int> stack;
	for (int i = 0; i < input.size(); i++)
		if (input[i] == left)
			stack.push(i);
		else if (input[i] == right && stack.size() > 0)
		{
			matchingPairs[stack.top()] = i;
			stack.pop();
		}
	return matchingPairs;
}
bool matchRegex(std::string op)
{
	if (operators.find(op) != operators.end())
		return operators.find(op) != operators.end();
	for (const auto& value : operators)
		if (op.find(value.first) != std::string::npos)
			return true;
	return false;
}
int getPrecedence(std::string op)
{
	auto it = operators.find(op);
	if (it != operators.end()) return std::get<0>(std::get<1>(*it));
	for (const auto& value : operators)
		if (op.find(value.first) != std::string::npos)
			return std::get<0>(value.second);
	return -1;
}
calcPointer getOperatorMethod(std::string op)
{
	auto it = operators.find(op);
	if (it != operators.end()) return std::get<3>(std::get<1>(*it));
	for (const auto& value : operators)
		if (op.find(value.first) != std::string::npos)
			return std::get<3>(value.second);
	return nullptr;
}
std::vector<regexMatch> getRegexMatches(std::string text, std::regex regex)
{
	std::vector<regexMatch> matches;
	std::smatch match;
	while (regex_search(text, match, regex))
	{
		matches.emplace_back(match.str(), match.position());
		text = match.suffix().str();
	}
	return matches;
}
bool isDecimal(std::string str)
{
	//Special operator -, can interfear with the method
	if (str == "-")
		return false;
	//If the first char is a -, then cut it of, as it is an operator and a valid value.
	if (str[0] == '-')
		str = str.substr(1);
	for (const char& ch : str)
		if (!isdigit(ch) && ch != '.')
			return false;
	return true;
}
operatorInfo getOperaInfo(const std::string op)
{
	const auto it{ operators.find(op) };
	if (it != operators.end())
		return it->second;
	for (const auto& value : operators)
		if (op.find(value.first) != std::string::npos)
			return value.second;
}
bool isSpecialOpera(const std::string op)
{
	return std::get<1>(getOperaInfo(op)) != "";
}
convertPointer getOperatorSpecial(std::string op)
{
	return std::get<4>(getOperaInfo(op));
}
std::queue<regexMatch> convertToPostfix(std::string& expression)
{
	if (check_convertToPostfix)
		std::cout << "<-----convertToPostfix----->\n";

	expr = expression;
	std::vector<regexPattern> searchRegex;
	for (const auto& regex : operators)
		searchRegex.push_back(std::get<2>(regex.second));
	searchRegex.emplace_back(R"(\d+(\.\d+)?)");
	searchRegex.emplace_back(R"([()])");
	//Make a big regex for all operators
	std::string allOperatorRegex;
	for (const auto& op : searchRegex)
		allOperatorRegex.append(op.pattern).append("|");
	allOperatorRegex.pop_back();
	//Get all of the matches in a collection (matches)

	std::vector<regexMatch> matches = getRegexMatches(expression, std::regex(allOperatorRegex));

	std::queue<regexMatch> outputQueue;
	std::stack<regexMatch> operatorStack;
	for (size_t i = 0; i < matches.size(); i++)
	{
		if (matches[i].value == "-" && (i == 0 || matchRegex(matches[i - 1].value) || matches[i - 1].value == "("))
		{
			outputQueue.emplace(regexMatch{ matches[i].value + matches[i + 1].value, matches[i].pos });
			i++;
			continue;
		}
		else if (isDecimal(matches[i].value))
			outputQueue.emplace(matches[i]);
		else if (matches[i].value == "(")
			operatorStack.push(matches[i]);
		else if (matches[i].value == ")")
		{
			while (operatorStack.size() > 0 && operatorStack.top().value != "(")
			{
				outputQueue.emplace(operatorStack.top());
				operatorStack.pop();
			}
			if (operatorStack.size() == 0 || operatorStack.top().value != "(")
				throw std::exception("Mismatched parentheses.");
			operatorStack.pop();
		}
		else
		{
			while (operatorStack.size() > 0 && matchRegex(operatorStack.top().value) && getPrecedence(matches[i].value) <= getPrecedence(operatorStack.top().value))
			{
				outputQueue.emplace(operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(matches[i]);
		}
	}
	while (operatorStack.size() > 0)
	{
		if (operatorStack.top().value == "(")
			throw std::exception("Mismatched parentheses.");
		outputQueue.emplace(operatorStack.top());
		operatorStack.pop();
	}

	///Testing
	if (check_convertToPostfix)
	{
		std::queue<regexMatch> testOutputQueue = outputQueue;
		while (testOutputQueue.size() > 0)
		{
			std::cout << testOutputQueue.front().value << std::endl;
			testOutputQueue.pop();
		}
	}

	return outputQueue;
}
long double evaluatePostfix(std::queue<regexMatch> postfixQueue)
{
	if (check_evaluatePostfix)
		std::cout << "<-----evaluatePostfix----->\n";
	std::stack<long double> valueStack;
	while (postfixQueue.size() > 0)
	{
		long double a{ NULL }, b{ NULL };
		const regexMatch match = postfixQueue.front();
		postfixQueue.pop();
		
		if (isDecimal(match.value))
			valueStack.push(std::stold(match.value));
		else if (matchRegex(match.value))
		{
			if (valueStack.size() < 1)
				throw std::exception("Invalid expression");
			a = valueStack.top();
			valueStack.pop();
			if (!isSpecialOpera(match.value))
			{
				b = valueStack.top();
				valueStack.pop();
			}
			else if (match.value.find(std::get<1>(getOperaInfo(match.value))) != std::string::npos)
				b = getOperatorSpecial(match.value)(match.value, match.pos);

			valueStack.push(getOperatorMethod(match.value)(a, b));

			if (check_evaluatePostfix)
				std::cout << valueStack.top() << std::endl;
		}
		else
			throw std::exception("Invalid operator in the expression");
	}
	if (valueStack.size() < 1)
		throw std::exception("Invalid expression");
	return valueStack.top();
}
void prepareUserValues()
{
	//Prepare settings
	for (const auto& neededSetting : neededSettings)
		if (SCalc::settings.find(neededSetting.first) == SCalc::settings.end())
			SCalc::settings[neededSetting.first] = neededSetting.second;


}
bool allLetters(const std::string& str)
{
	for (char c : str)
		if (!(isalpha(c) || c == '(' || c == ')' || c == '[' || c == ']'))
			return false;
	return true;
}
bool hasNumbers(const std::string& str)
{
	return std::any_of(str.begin(), str.end(), [](char c) { return !std::isalpha(c); });
}
bool compareByFirst(const std::pair<const char*, long double>& a, const std::pair<const char*, long double>& b)
{
	return strlen(a.first) > strlen(b.first);
}
void replaceAll(std::string& originalString, const std::string& searchString, const std::string& replacementString)
{
	size_t searchLen = searchString.length();
	size_t replaceLen = replacementString.length();
	size_t pos = originalString.find(searchString);
	while (pos != std::string::npos)
	{
		originalString.replace(pos, searchLen, replacementString);
		pos = originalString.find(searchString, pos + replaceLen);
	}
}



//Custom Math
long double logb(long double x, long double b)
{
	if (x <= 0 || b <= 0 || b == 1)
		return NAN;
	return log(x) / log(b);
}



