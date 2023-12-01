#include <iostream>
#include "Calculator/SCalc.h"
#include <chrono>
#include <iomanip> 

int main()
{
	SCalc::settings = 
	{
		{ "#Decimals", {4, NULL} },
		{ "Normal/LaTeX/MML/OMML", {NULL, NULL} },
		{ "Radians", {NULL, true} },
	};
	std::cout << std::setprecision(SCalc::settings["#Decimals"].first);

	SCalc::values =
	{
		{ "a", 4.29 },
		{ "b", 0.5 },
		{ "c", -1.5 },
	};
	auto start = std::chrono::high_resolution_clock::now();
	std::string expression1 = "(-b + sqrt(b^2 - 4 * a * c))/(2 * a)";
	std::cout << "Result: " << SCalc::calculate(SCalc::giveValues(expression1, SCalc::values)) << std::endl;
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Elapsed: " << (double)std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000 << " milliseconds." << std::endl;
}