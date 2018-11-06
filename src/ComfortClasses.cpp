/*
 * ComfortClasses.cpp
 *
 *  Created on: 24.10.2018
 *      Author: jan
 */

#include <iostream>
#include <string>

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

int main(){
	std::string zeroStr("+0.0000");
	CCDecimal zero(zeroStr);
	std::cout << zeroStr << " liefert " << zero.toString() << std::endl;
	std::string juhuStr("-000123000000123.0003740000");
		CCDecimal juhu(juhuStr);
		std::cout << juhuStr << " liefert " << juhu.toString() << std::endl;

}

