#include "SCalc_Logic.h"
#include "SCalc.h"
#include <cmath>
constexpr auto PI = 3.141592653589793238462643383279502884L;
#define degrees(r) r * (180.0 / PI)
#define radians(d) d * (PI / 180.0)


//Operator Calculators
long double sqrtOperator(const long double& a, const long double& b)
{
	if (b == NULL)
		return std::sqrt(a);
	return std::pow(a, 1 / b);
}
long double logOperator(const long double& a, const long double& b)
{
	if (b == NULL)
		return std::log10(a);
	return logb(a,b);
}
long double sinOperator(const long double& a, const long double& b)
{
	if (SCalc::settings["Radians"].second)
	{
		if (b == NULL)
			return std::sin(a);
		return std::asin(a);
	}
	else
	{
		if (b == NULL)
			return std::sin(a * (PI / 180.0));
		return degrees(std::asin(a));
	}
}
long double cosOperator(const long double& a, const long double& b)
{
	if (SCalc::settings["Radians"].second)
	{
		if (b == NULL)
			return std::cos(a);
		return std::acos(a);
	}
	else
	{
		if (b == NULL)
			return std::cos(a * (PI / 180.0));
		return degrees(std::acos(a));
	}
}
long double tanOperator(const long double& a, const long double& b)
{
	if (SCalc::settings["Radians"].second)
	{
		if (b == NULL)
			return std::tan(a);
		return std::atan(a);
	}
	else
	{
		if (b == NULL)
			return std::tan(a * (PI / 180.0));
		return degrees(std::atan(a));
	}
}
long double potensOperator(const long double& a, const long double& b)
{
	return std::pow(b, a);
}
long double multiplyOperator(const long double& a, const long double& b)
{
	return b * a;
}
long double divideOperator(const long double& a, const long double& b)
{
	return b / a;
}
long double plusOperator(const long double& a, const long double& b)
{
	return b + a;
}
long double minusOperator(const long double& a, const long double& b)
{
	return b - a;
}

//Special Operator Format
long double sqrtSpecial(const std::string& opera, const std::size_t& pos)
{
	std::size_t endOfSpecial{ pos + opera.length() - 1 };
	std::unordered_map<int, int> parPairs{ matchParentheses(expr, '[', ']') };
	return std::stold(expr.substr(endOfSpecial + 1, parPairs[int(endOfSpecial)] - endOfSpecial - 1));
}
long double logSpecial(const std::string& opera, const std::size_t& pos)
{
	std::size_t endOfSpecial{ pos + opera.length() - 1 };
	std::unordered_map<int, int> parPairs{ matchParentheses(expr, '[', ']') };
	return std::stold(expr.substr(endOfSpecial + 1, parPairs[int(endOfSpecial)] - endOfSpecial - 1));
}
long double sinSpecial(const std::string& opera, const std::size_t& pos)
{
	std::size_t endOfSpecial{ pos + opera.length() - 1 };
	std::unordered_map<int, int> parPairs{ matchParentheses(expr, '[', ']') };
	return std::stold(expr.substr(endOfSpecial + 1, parPairs[int(endOfSpecial)] - endOfSpecial - 1));
}
long double cosSpecial(const std::string& opera, const std::size_t& pos)
{
	std::size_t endOfSpecial{ pos + opera.length() - 1 };
	std::unordered_map<int, int> parPairs{ matchParentheses(expr, '[', ']') };
	return std::stold(expr.substr(endOfSpecial + 1, parPairs[int(endOfSpecial)] - endOfSpecial - 1));
}
long double tanSpecial(const std::string& opera, const std::size_t& pos)
{
	std::size_t endOfSpecial{ pos + opera.length() - 1 };
	std::unordered_map<int, int> parPairs{ matchParentheses(expr, '[', ']') };
	return std::stold(expr.substr(endOfSpecial + 1, parPairs[int(endOfSpecial)] - endOfSpecial - 1));
}


bool check_convertToPostfix = false;
bool check_evaluatePostfix = false;

operatorMap operators =
{
	//Operator		Precedence		Is Special			Operator Regex				Calculation Logic			Get Special
	{ "sqrt",		{4,				"[",				{R"(sqrt(\[)?)"},			sqrtOperator,				sqrtSpecial} },
	{ "log",		{4,				"[",				{R"(log(\[)?)"},			logOperator,				logSpecial} },
	{ "sin",		{4,				"[",				{R"(sin(\[)?)"},			sinOperator,				sinSpecial} },
	{ "cos",		{4,				"[",				{R"(cos(\[)?)"},			cosOperator,				cosSpecial} },
	{ "tan",		{4,				"[",				{R"(tan(\[)?)"},			tanOperator,				tanSpecial} },
	{ "^",			{3,				"",					{R"(\^)"},					potensOperator,				NULL} },
	{ "*",			{2,				"",					{R"(\*)"},					multiplyOperator,			NULL} },
	{ "/",			{2,				"",					{R"(\/)"},					divideOperator,				NULL} },
	{ "+",			{1,				"",					{R"(\+)"},					plusOperator,				NULL} },
	{ "-",			{1,				"",					{R"(\-)"},					minusOperator,				NULL} },
};

std::unordered_map<std::string, std::pair<int, bool>> neededSettings =
{
	{ "#Decimals", {3, NULL} },
	{ "Format Formula", {NULL, true} },
	{ "Normal/LaTeX/MML/OMML", {0, NULL} },
	{ "ShowEqualSign", {NULL, true} },
	{ "Radians", {NULL, true} },
};













/*Time taking
auto startTime = std::chrono::high_resolution_clock::now();
auto endTime = std::chrono::high_resolution_clock::now();
std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " Milliseconds" << std::endl;
*/