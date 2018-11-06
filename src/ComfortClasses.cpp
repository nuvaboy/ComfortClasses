/*
 * ComfortClasses.cpp
 *
 *  Created on: 24.10.2018
 *      Author: jan
 */

#include <iostream>
#include <string>

int main() {
	std::string str;

	for (char c = '0'; c <= '9'; c++) {
		str.push_back(c);
	}

	std::cout << str << std::endl;
	for (auto rit = str.rbegin(); rit != str.rend(); rit++) {
		std::cout << *rit;
		if (*rit == '5') {
			std::cout << " removing " << rit.base()[-1] << " ";
			str.erase(rit.base()-1);
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



