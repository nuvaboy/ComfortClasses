/*
 * ComfortClasses.cpp
 *
 *  Created on: 24.10.2018
 *      Author: jan
 */

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>

#include "CCDecimal.h"

void deleteTest() {
	std::string str;

	for (char c = '0'; c <= '9'; c++) {
		str.push_back(c);
	}

	std::cout << str << std::endl;
	for (auto rit = str.rbegin(); rit != str.rend(); rit++) {
		std::cout << *rit;
		if (*rit == '5') {
			std::cout << " removing " << rit.base()[-1] << " ";
			str.erase(rit.base() - 1);
		}
		std::cout << *rit << std::endl;

	}

	std::cout << std::endl << str << std::endl;

	str.clear();

	for (char c = '0'; c <= '9'; c++) {
		str.push_back(c);
	}

	std::cout << str << std::endl;
	for (auto it = str.begin(); it != str.end(); it++) {
		std::cout << *it;
		if (*it == '5') {
			std::cout << " removing " << *it << " ";
			str.erase(it);
		}
		std::cout << *it << std::endl;

	}

	std::cout << std::endl << str << std::endl;

	std::cin >> str;
}

int main() {
	//deleteTest();
	std::vector<std::string> testCasesStr = {
		"000987",
		"+1098765432109876543210987654321000",
		".7",
		"-.00036000",
		"+40.",
		"-52.79876543210987654321098765432"
	};
	for (std::string testCase : testCasesStr) {
		CCDecimal test(testCase);
		std::cout << testCase << " liefert " << test.toString() << std::endl;
	}
	std::vector<double> testCasesD = {
		000987.,
		+1098765432109876543210987654321000.,
		.7,
		-.00036000,
		+40.,
		-52.79876543210987654321098765432
	};
	for (double testCase : testCasesD) {
		CCDecimal test(testCase);
		std::cout << std::setprecision(std::numeric_limits<double>::digits10) << testCase << " liefert " << test.toString() << std::endl;
	}
	std::string input;
	std::cin >> input;
}

