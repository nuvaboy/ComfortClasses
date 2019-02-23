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

	//TODO: Remove this in release version (for testing purposes only)
	friend class CCDecimalTest;

private:

	//### attributes ##################################
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

	/**
	 * \brief globale Präzision
	 *
	 * Die globale Präzision wirkt sich auf alle Instanzen von CCDecimal aus,
	 * deren lokale Präzision #localPrecision nicht explizit gesetzt wurde.
	 * Die Präzision wird für die entsprechende (Anzahl-1) an Nachkommastellen garantiert.
	 * Es wird eine Stelle weniger garantiert, da diese intern für ein korrektes Runden benötigt wird.
	 */
	static int32_t globalPrecision;

	//### utility funcions ############################
	/** \brief Erzeugt einen CCDecimal aus einer String-Darstellung.
	 *
	 * Hilfsfunktion, welche den CCDecimal entsprechend einer gegebenen String-Darstellung anlegt.
	 * Es wird davon ausgegangen, dass dieser zuvor mit 0 initialisiert wurde.
	 *
	 * @param numberStr string, welcher in den CCDecimal umgewandelt wird.
	 * @throws overflow_error Der Wertebereich wurde überschritten. (Mantisse kann maximal #MAX Ziffern halten)
	 * @throws invalid_argument Die Dezimalzahl hat ein ungültiges Format.
	 *
	 */
	void constructFromString(string numberStr);
	bool magnitudeLessThan(const CCDecimal& op2) const;
	void add(CCDecimal* result, const CCDecimal& op2) const;
	void sub(CCDecimal* result, const CCDecimal& opSmall) const;
	void mult(CCDecimal* result, const CCDecimal& op2) const;
	void div(CCDecimal* result, const CCDecimal& op2) const;
	void mod(CCDecimal* result, const CCDecimal& op2) const;

public:

	//### constructors/destructors #####################
	/** \brief Konstruktor (default)
	 *
	 *  Erstellt ein CCDecimal mit dem Wert 0.
	 **/
	CCDecimal() noexcept;

	/** \brief Konstruktor (copy)
	 *
	 *  Erstellt eine Kopie eines anderen CCDecimals.
	 *  Der Zeiger pPrecision verweist entweder auf die lokal oder global definierte Präzision.
	 *  Wurde die lokale Präzision 'precision' explizit für das Original gesetzt,
	 *  muss der Zeiger 'pPrecision' auf die lokale Präzision der Kopie gesetzt werden.
	 *  Andernfalls wird die globale Präzision als "shallow copy" übernommen.
	 *
	 *
	 *  @param original Referenz des CCDecimals, welcher kopiert wird
	 */
	CCDecimal(const CCDecimal& original);

	/** \brief Konstruktor (double)
	 *
	 *  Erstellt einen CCDecimal auf Basis des übergebenen doubles.
	 *  Durch Einlesen in einen 'stringstream' wird eine String-Repräsentation erzeugt.
	 *  Die Verwendung von 'setPrecision' stellt sicher, dass alle vorhandenen Nachkommastellen übernommen werden.
	 *  Abschließend konstruiert #constructFromString den CCDecimal.
	 *
	 *  @param number double, aus dem ein CCDecimal erzeugt wird
	 * */
	CCDecimal(double number);

	/** \brief Konstruktor (std::string)
	 *
	 *  Erstellt mithilfe von #constructFromString einen CCDecimal auf Basis des übergebenen std::string.
	 *
	 *  @param numberStr std::string, aus dem ein CCDecimal erzeugt wird
	 */
	CCDecimal(const string& numberStr);

	/** \brief Konstruktor (C-String)
	 *
	 *	Konstruiert  einen CCDecimal von einem C-String.
	 *
	 * @param numberCStr C-String aus dem ein CCDecimal erzeugt wird
	 */
	CCDecimal(const char* numberCStr);

	/** \brief Destruktor
	 *
	 *  Existiert um gegebenenfalls Vererbung zu ermöglichen.
	 */
	virtual ~CCDecimal();

	//### public setter/getter #########################
	/** \brief Liefert die aktuelle Präzision zurück.
	 *
	 *  Liefert die lokale Präzision #localPrecision oder
	 *  die globale Präzision #globalPrecision, solange die lokale nicht gesetzt wurde.
	 *  Der Zeiger #pPrecision verweist entsprechend auf die lokale/globale Präzision.
	 *
	 * @return Präzision des CCDecimal's
	 */
	int32_t getPrecision();

	/** \brief Setzt die lokale Präzision.
	 *  Aktualisiert den Zeiger #pPrecision, damit dieser auf die lokale Präzision #localPrecision verweist.
	 *
	 * @param precision Präzision, die lokal für diesen CCDecimal festgelegt wird ( >= 0 )
	 * @throws out_of_range 'precision' muss positiv sein.
	 */
	void setLocalPrecision(int32_t precision);

	/** \brief Liefert die globale Präzision zurück.
	 *
	 *  Liefert die globale Präzision #globalPrecision zurück.
	 *   *
	 * @return die globale Präzision
	 */
	static int32_t getGlobalPrecision();

	/** \brief Setzt die globale Präzision.
	 *
	 * @see localPrecision
	 * @param precision globale Präzision
	 * @throws out_of_range 'precision' muss positiv sein.
	 */
	static void setGlobalPrecision(int32_t);

	//### arithmetic operators #########################
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

	//### comparison operators #########################
	bool operator ==(const CCDecimal&) const;
	bool operator !=(const CCDecimal&) const;
	bool operator <(const CCDecimal&) const;
	bool operator >(const CCDecimal&) const;

	//### conversion functions #########################
	/** \brief Rundet eine CCDecimal
	 *
	 * Rundet auf 'precOut' Nachkommastellen genau.
	 * Sind mehr Nachkommastellen vorhanden wird aufgerundet,
	 * wenn die erste abgeschnittene Ziffer >= 5 ist. Andernfalls wird abgerundet.
	 *
	 * @param pDec Zeiger auf die zu rundende CCDecimal
	 * @param precOut Genauigkeit (in Nachkommastellen, >= 0)
	 * @throws out_of_range 'precision' muss positiv sein.
	 *
	 */
	static void round(CCDecimal* pDec, int32_t precOut);

	/** \brief Liefert String-Darstellung mit Präzision und optional wissenschaftlicher Notation.
	 *
	 * Konvertiert eine CCDecimal in einen String und
	 * rundet entsprechend der angegebenen Anzahl an Nachkommastellen 'precOut'. (#round)
	 * Optional kann die wissenschaftliche Notation gewählt werden.
	 *
	 * @param precOut Anzahl an Nachkommastellen, nach denen gerundet wird.
	 * @param scientific Flag, welches die wissenschaftliche Notation aktiviert.
	 * @return string, der die Dezimalzahl repräsentiert
	 */
	string toString(int32_t precOut, bool scientific = false) const;

	/** \brief Liefert String-Darstellung mit optional wissenschaftlicher Notation.
	 *
	 * Komfortfunktion, welche toString ohne Angabe der Präzision aufruft.
	 * @see int32_t toString(precOut, bool scientific = false)
	 *
	 * @param scientific Flag, welches die wissenschaftliche Notation aktiviert.
	 * @return string, der die Dezimalzahl repräsentiert
	 */
	string toString(bool scientific = false) const;

	/** \brief Liefert einen double zurück.
	 *
	 * Bei der Konvertierung von CCDecimal zu double kann Präzision verloren gehen.
	 * Unabhängig von der grantierten Präzision #pPrecision wird die beste Annährung angestrebt.
	 *
	 * @return double (beste Annährung an den CCDecimal)
	 */
	double toDouble() const;

//	CCDecimal pow(int32_t exp){
//
//		if (exp < 0) exp = -exp;
//		int arr_size = 0;
//		while (exp > 0){
//			exp >>= 1;
//			arr_size++;
//		}
//
//		//2048
//		CCDecimal* arr = new CCDecimal[arr_size];
//		delete[] arr;
//
//		return CCDecimal();
//
//		//^10
//		//*=2
//		//*=2
//		//*=2
//		//5*5*5*5*5*5*5*5*5*5*5
//		//2
//		//4
//		//8
//		//16
//		//32
//		//64
//		//128
//		//256
//		//512
//		//1024
//		//2048
//		//4092
//		//10 x 8  = 80 Byte
//
//	}

};

//### stream operator ##############################

std::ostream& operator<<(std::ostream &os, const CCDecimal& dec);

#endif /* CCDECIMAL_H_ */
