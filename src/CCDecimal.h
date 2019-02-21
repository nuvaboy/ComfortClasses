/*
 * Decimal.h
 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */


#ifndef CCDECIMAL_H_
#define CCDECIMAL_H_

#define MAX 31 //31

#include <stdint.h>
#include <iostream>
#include <cstdarg>
#include <string>

using namespace std;

class CCDecimal {

private:

	/**
	 * \brief Array von Koeffizienten der Dezimalzahl
	 *
	 * Digit ist ein Array, welches die Koeffizienten der Dezimalzahl verwaltet.
	 * Die maximale Kapazität ist durch das Define #MAX vorgegeben.
	 * Es wird jedocch eine weitere Stelle für den Fall eines Overflows reserviert.
	 */
	int8_t digit[MAX + 1];

	/**
	 * \brief Anzahl belegter Dezimalstellen
	 *
	 * Used ist die Anzahl belegter Dezimalstellen, welche höstens den Wert (#MAX+1) erreichen kann.
	 * Werden jedoch (#MAX+1) Ziffern belegt, ist ein Overflow aufgetreten.
	 */
	uint32_t used = 0;

	/**
	 * \brief Wertigkeit des LSDs
	 *
	 * Shift beschreibt die Wertigkeit des LSDs (Least Significant Digit).
	 * Entsprechend besitzt eine Dezimalzahl mit einem shift von -2 zwei Nachkommastellen
	 * und aus einem shift von +5 folgen 5 nachfolgende Nullen.
	 */
	int32_t shift = 0;

	/**
	 * \brief Vorzeichen der Dezimalzahl
	 *
	 * Dieses Flag zeigt an, ob die Dezimalzahl negativ ist.
	 */
	bool isNegative = false;

	/**
	 * \brief lokale Präzision
	 *
	 * Ist die lokale Präzision eines CCDecimal's gesetzt wird sichergestellt,
	 * dass diese für die entsprechende (Anzahl-1) an Nachkommastellen erhalten bleibt.
	 * Andernfalls wird die globale Präzision #globalPrecision garantiert.
	 * Es wird eine Stelle weniger garantiert, da diese intern für ein korrektes Runden benötigt wird.
	 */
	int32_t localPrecision = 0;

	/**
	 * \brief Zeiger auf aktuelle Präzision
	 *
	 * Der Zeiger pPrecison verweist auf die lokale Präzision #localPrecision
	 * oder die globale Präzision #globalPrecision, wenn erstere nicht explizit gesetzt wurde.
	 */
	int32_t* pPrecision;


	static int32_t globalPrecision;

	void constructFromString(string numberStr);
	bool magnitudeLessThan(const CCDecimal& op2) const;
	void add(CCDecimal* result, const CCDecimal& op2) const;
	void sub(CCDecimal* result, const CCDecimal& opSmall) const;
	void mult(CCDecimal* result, const CCDecimal& op2) const;
	void div(CCDecimal* result, const CCDecimal& op2) const;
	void mod(CCDecimal* result, const CCDecimal& op2) const;


public:

	//constructors
	CCDecimal() noexcept;
	CCDecimal(const CCDecimal& d2);
	CCDecimal(const char* str);
	CCDecimal(const string& numberStr);
	CCDecimal(double number);
	virtual ~CCDecimal();

	int32_t getPrecision();
	void setLocalPrecision(int32_t prec);
	static void setGlobalPrecision(int32_t);
	static int32_t getGlobalPrecision();

	CCDecimal operator +(const CCDecimal&) const;
	CCDecimal operator -(const CCDecimal&) const;
	CCDecimal operator *(const CCDecimal&) const;
	CCDecimal operator /(const CCDecimal&) const;
	CCDecimal operator %(const CCDecimal&) const;

	CCDecimal& operator +=(const CCDecimal&);
	CCDecimal& operator -=(const CCDecimal&);
	CCDecimal& operator *=(const CCDecimal&);
	CCDecimal& operator /=(const CCDecimal&);
	CCDecimal& operator %=(const CCDecimal&);

	CCDecimal& operator++();
	CCDecimal& operator--();
	CCDecimal operator++(int);
	CCDecimal operator--(int);

	bool operator ==(const CCDecimal&) const;
	bool operator !=(const CCDecimal&) const;
	bool operator <(const CCDecimal&) const;
	bool operator >(const CCDecimal&) const;

	string toString(int32_t precOut, bool scientific = false) const;
	string toString(bool scientific = false) const;
	double toDouble() const;
	static void round(CCDecimal* pDec, int32_t precOut);



	//testing only
	void setDigit(unsigned int pos, int8_t value) {
		digit[pos] = value;
		if (pos >= used && value != 0) {
			used = pos + 1;
		}
	}

	void setNegative(bool isNegative) {
		this->isNegative = isNegative;
	}

	void setDigits(int count, ...) {
		//cout << count << endl;

		va_list arguments;             // A place to store the list of arguments
		va_start(arguments, count); // Initializing arguments to store all values after count
		for (int i = count - 1; i >= 0; i--) {
			int val = (int) (va_arg(arguments, int));
			setDigit(i, val);
			//cout << va_arg(arguments, int) << endl;
		}
		// Adds the next value in argument list to sum.
		va_end(arguments);

	}

	void setShift(int shift) {
		this->shift = shift;
	}
//	/** @} */

};

std::ostream& operator<<(std::ostream &os, const CCDecimal& dec);


#endif /* CCDECIMAL_H_ */
