/*
 * Decimal.h
 *
 *  Created on: 24.10.2018
 *      Author: marlo
 */

/**
 * @file    CCDecimal.hpp
 *
 * @brief   Implementation einer Dezimalzahl mit Gleitkomma-Arithmetik.
 *
 * @author  Marlo Siebolts
 */


#ifndef CCDECIMAL_H_
#define CCDECIMAL_H_

#include <cstdint>
#include <iostream>
#include <string>


#include "CCString.hpp"

class CCString;

using namespace std;

/**
 * @brief Implementation einer Dezimalzahl mit Gleitkomma-Arithmetik.
 *
 *
 */
class CCDecimal {

	//TODO: Remove this in release version (for testing purposes only)
	friend class CCDecimalTest;

private:

	/**
	 * \brief globale Präzision
	 *
	 * Die globale Präzision wirkt sich auf alle Instanzen von CCDecimal aus,
	 * deren lokale Präzision #localPrecision nicht explizit gesetzt wurde.
	 * Die Präzision wird für die entsprechende (Anzahl-1) an Nachkommastellen garantiert.
	 * Es wird eine Stelle weniger garantiert, da diese intern für ein korrektes Runden benötigt wird.
	 */
	static int32_t globalPrecision;

	 /** \brief Länge der Mantisse
	  * Legt fest, wie viele Dezimalstellen im Array #digit abgespeichert werden können.
	  *
	  */
	static const int32_t MAX = 31;

	//### attributes ##################################
	/**
	 * \brief Array von Koeffizienten der Dezimalzahl
	 *
	 * Digit ist ein Array, welches die Koeffizienten der Dezimalzahl verwaltet.
	 * Die maximale Kapazität ist durch das Define #MAX vorgegeben.
	 * Es wird jedocch eine weitere Stelle für den Fall eines Overflows reserviert.
	 */
	int8_t digit[CCDecimal::MAX + 1] = { 0 };

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

	/** \brief Vergleicht zwei Dezimalzahlen ohne Berücksichtigung des Vorzeichens.
	 *
	 * Es wird 'true' zurückgegeben,
	 * wenn die Magnitude (Betrag) der Dezimalzahl kleiner als die Magnitude der übergebenen Dezimalzahl ist.
	 *
	 * @param op2 der zu vergleichende CCDecimal	 *
	 * @return bool (Ergebnis des Vergleichs)
	 *
	 */
	bool magnitudeLessThan(const CCDecimal& op2) const;

	/** \brief Addiert eine Dezimalzahl, ohne Berücksichtigung des Vorzeichens
	 *
	 * Es wird der CCDecimal 'op2' addiert und das Ergbebnis in 'result' abgelegt.
	 * Das Vorzeichen wird nicht berücksichtigt.
	 * Es wird erwartet, dass der Wert von 'result' mit 0 initialisiert wurde.
	 *
	 * Kann das Ergebnis nicht abgespeichert werden,
	 * weil mehr Stellen nötig sind als die Kapazität #MAX und die geforderte Präzision #pPrecision zulassen,
	 * ist das Overflow-Flag (#digit[#MAX]) größer als 0.
	 *
	 * Tritt ein Overflow um eine einzige Stelle auf, ist sichergestellt, dass die restlichen Stellen korrekt sind.
	 * Andernfalls kann (bis auf das Overflow-Flag) keine Aussage über den internen Zustand von 'result' getroffen werden.
	 *
	 * @param op2 der CCDecimal, der addiert werden soll
	 * @param result Zeiger auf einen CCDecimal in dem das Ergebnis abgelegt werden soll (muss mit 0 initialisiert sein)
	 *
	 */
	void add(CCDecimal* result, const CCDecimal& op2) const;

	/** \brief Subtrahiert eine Dezimalzahl ohne Berücksichtigung des Vorzeichens.
	 *
	 * Es wird der CCDecimal 'opSmall' subtrahiert und das Ergbebnis in 'result' abgelegt.
	 * Das Vorzeichen wird nicht berücksichtigt.
	 * Es wird erwartet, dass 'opSmall' die kleinere der beiden Dezimalzahlen ist
	 * und der Wert von 'result' mit 0 initialisiert wurde.
	 *
	 * Kann das Ergebnis nicht abgespeichert werden,
	 * weil mehr Stellen nötig sind als die Kapazität #MAX und die geforderte Präzision #pPrecision zulassen,
	 * ist das Overflow-Flag (#digit[#MAX]) größer als 0.
	 *
	 * Tritt ein Overflow um eine einzige Stelle auf, ist sichergestellt, dass die restlichen Stellen korrekt sind.
	 * Andernfalls kann (bis auf das Overflow-Flag) keine Aussage über den internen Zustand von 'result' getroffen werden.
	 *
	 * @param opSmall der CCDecimal, der subtrahiert werden soll (muss die kleinere der beiden Dezimalzahlen sein)
	 * @param result Zeiger auf einen CCDecimal in dem das Ergebnis abgelegt werden soll  (muss mit 0 initialisiert sein)
	 */
	void sub(CCDecimal* result, const CCDecimal& opSmall) const;

	/** \brief Multipliziert mit einer Dezimalzahl ohne Berücksichtigung des Vorzeichens.
	 *
	 * Es wird mit dem CCDecimal 'op2' multipliziert und das Ergbebnis in 'result' abgelegt.
	 * Das Vorzeichen wird nicht berücksichtigt.
	 * Es wird erwartet, dass der Wert von 'result' mit 0 initialisiert wurde.
	 *
	 * Kann das Ergebnis nicht abgespeichert werden,
	 * weil mehr Stellen nötig sind als die Kapazität #MAX und die geforderte Präzision #pPrecision zulassen,
	 * ist das Overflow-Flag (#digit[#MAX]) größer als 0.
	 *
	 * Tritt ein Overflow um eine einzige Stelle auf, ist sichergestellt, dass die restlichen Stellen korrekt sind.
	 * Andernfalls kann (bis auf das Overflow-Flag) keine Aussage über den internen Zustand von 'result' getroffen werden.
	 *
	 * @param op2 der CCDecimal, mit dem multipliziert werden soll
	 * @param result Zeiger auf einen CCDecimal in dem das Ergebnis abgelegt werden soll (muss mit 0 initialisiert sein)
	 */
	void mult(CCDecimal* result, const CCDecimal& op2) const;

	/** \brief Dividiert durch eine Dezimalzahl ohne Berücksichtigung des Vorzeichens.
	 *
	 * Es wird durch den CCDecimal 'op2' dividiert und das Ergbebnis in 'result' abgelegt.
	 * Das Vorzeichen wird nicht berücksichtigt.
	 * Es wird erwartet, dass der Wert von 'result' mit 0 initialisiert wurde.
	 *
	 * Kann das Ergebnis nicht abgespeichert werden,
	 * weil mehr Stellen nötig sind als die Kapazität #MAX und die geforderte Präzision #pPrecision zulassen,
	 * ist das Overflow-Flag (#digit[#MAX]) größer als 0.
	 *
	 * Tritt ein Overflow um eine einzige Stelle auf, ist sichergestellt, dass die restlichen Stellen korrekt sind.
	 * Andernfalls kann (bis auf das Overflow-Flag) keine Aussage über den internen Zustand von 'result' getroffen werden.
	 *
	 * @param op2 der CCDecimal, durch den dividiert werden soll
	 * @param result Zeiger auf einen CCDecimal in dem das Ergebnis abgelegt werden soll (muss mit 0 initialisiert sein)
	 * @throws domain_error Division durch 0
	 */
	void div(CCDecimal* result, const CCDecimal& op2) const;

	/** \brief Berechnet Rest der Division durch eine Dezimalzahl ohne Berücksichtigung des Vorzeichens.
	 *
	 * Es wird der Rest der Division durch den CCDecimal 'op2' berechnet und in 'result' abgelegt.
	 * Das Vorzeichen wird nicht berücksichtigt.
	 * Es wird erwartet, dass der Wert von 'result' mit 0 initialisiert wurde.
	 *
	 * Diese Operation kann keinen Overflow erzeugen.
	 *
	 * Tritt ein Overflow um eine einzige Stelle auf, ist sichergestellt, dass die restlichen Stellen korrekt sind.
	 * Andernfalls kann (bis auf das Overflow-Flag) keine Aussage über den internen Zustand von 'result' getroffen werden.
	 *
	 * @param op2 der CCDecimal, durch den dividiert werden soll
	 * @param result Zeiger auf einen CCDecimal in dem das Ergebnis abgelegt werden soll (muss mit 0 initialisiert sein)
	 * @throws domain_error Division durch 0
	 */
	void mod(CCDecimal* result, const CCDecimal& op2) const;

public:

	//### constructors/destructors #####################
	/** \brief Konstruktor (default)
	 *
	 *  Erstellt ein CCDecimal mit dem Wert 0.
	 */
	CCDecimal();

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
	 */
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
	 *	Konstruiert  einen CCDecimal aus einem C-String.
	 *
	 * @param numberCStr C-String aus dem ein CCDecimal erzeugt werden soll.
	 */
	CCDecimal(const char* numberCStr);

	/** \brief Konstruktor (CCString)
	 *
	 *  Konstruiert einen CCDecimal aus einem CCString.
	 *
	 *  @param ccStr CCString aus dem ein CCDecimal erzeugt werden soll.
	 */
	CCDecimal(const CCString& ccStr);

	/** \brief Destruktor
	 *
	 *  Existiert um gegebenenfalls Vererbung zu ermöglichen.
	 */
	virtual ~CCDecimal() = default;

	/** \brief Kopierzuweisung
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
    CCDecimal& operator=(const CCDecimal& original);

	//### public setter/getter #########################
	/** \brief Liefert die aktuelle Präzision zurück.
	 *
	 *  Liefert die lokale Präzision #localPrecision oder
	 *  die globale Präzision #globalPrecision, solange die lokale nicht gesetzt wurde.
	 *  Der Zeiger #pPrecision verweist entsprechend auf die lokale/globale Präzision.
	 *
	 * @return Präzision des CCDecimal's
	 */
	int32_t getPrecision() const;

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
	/** \brief Addiert eine Dezimalzahl.
	 *
	 * Bildet die Addition in Abhängigkeit der Vorzeichen auf die Hilfsfunktionen #sub und #add ab und korrigiert das Vorzeichen im Ergebnis.
	 * Zahlen mit gleichen Vorzeichen werden addiert. (Vorzeichen wird unverändert übernommen)
	 * Ansonsten wird die Zahl mit dem größeren Betrag von der anderen abgezogen und deren Vorzeichen übernommen.
	 *
	 * @param op2 der CCDecimal, der addiert werden soll
	 * @return CCDecimal (Ergebnis der Addition)
	 * @throws overflow_error Ergebnis der Addition kann nicht abgespeichert werden.
	 */
	CCDecimal operator +(const CCDecimal& op2) const;

	/** \brief Subtrahiert eine Dezimalzahl.
	 *
	 * Bildet die Subtraktion in Abhängigkeit der Vorzeichen auf die Hilfsfunktionen #sub und #add ab und korrigiert das Vorzeichen im Ergebnis.
	 * Zahlen mit ungleichen Vorzeichen werden addiert. (Vorzeichen wird unverändert übernommen)
	 * Ansonsten wird die Zahl mit dem kleineren Betrag von der anderen abgezogen.
	 * Hat 'op2' den kleineren Betrag bleibt das Vorzeichen unverändert. Andernfalls wird das Vorzeichen umgekehrt.
	 *
	 * @param op2 der CCDecimal, der subtrahiert werden soll
	 * @return CCDecimal (Ergebnis der Subtraktion)
	 * @throws overflow_error Ergebnis der Subtraktion kann nicht abgespeichert werden.
	 *
	 */
	CCDecimal operator -(const CCDecimal& op2) const;

	/** \brief Multipliziert mit einer Dezimalzahl.
	 *
	 * Verwendet die Hilfsfunktionen #mult und passt anschließend das Vorzeichen an.
	 * Das Vorzeichen wird negativ wenn die Vorzeichen der Operanden sich unterscheiden.
	 *
	 * @param op2 der CCDecimal, mit dem multipliziert werden soll
	 * @return CCDecimal (Ergebnis der Multiplikation)
	 * @throws overflow_error Ergebnis der Multiplikation kann nicht abgespeichert werden.
	 */
	CCDecimal operator *(const CCDecimal& op2) const;

	/** \brief Dividiert durch eine Dezimalzahl.
	 *
	 * Verwendet die Hilfsfunktionen #div und passt anschließend das Vorzeichen an.
	 * Das Vorzeichen wird negativ wenn die Vorzeichen der Operanden sich unterscheiden.
	 *
	 * @param op2 der CCDecimal, durch den dividiert werden soll
	 * @return CCDecimal (Ergebnis der Division)
	 * @throws overflow_error Ergebnis der Division kann nicht abgespeichert werden.
	 *
	 */
	CCDecimal operator /(const CCDecimal& op2) const;

	/** \brief Berechnet Rest der Division durch eine Dezimalzahl.
	 *
	 * Verwendet die Hilfsfunktionen #mod und passt anschließend das Vorzeichen an.
	 * Das Vorzeichen wird negativ wenn die Vorzeichen der Operanden sich unterscheiden.
	 *
	 * @param op2 der CCDecimal, durch den dividiert werden soll
	 * @return CCDecimal (Rest der Division)
	 *
	 */
	CCDecimal operator %(const CCDecimal& op2) const;

	/** \brief Addition einer Dezimalzahl.
	 *
	 * @see #operator +
	 *
	 */
	CCDecimal& operator +=(const CCDecimal& op2);

	/** \brief Subtraktion einer Dezimalzahl.
	 *
	 * @see #operator -
	 *
	 */
	CCDecimal& operator -=(const CCDecimal& op2);

	/** \brief Multiplikation mit einer Dezimalzahl.
	 *
	 * @see #operator *
	 *
	 */
	CCDecimal& operator *=(const CCDecimal& op2);

	/** \brief Division durch eine Dezimalzahl.
	 *
	 * @see #operator /
	 *
	 */
	CCDecimal& operator /=(const CCDecimal& op2);

	/** \brief Berchnet Rest einer Division durch eine Dezimalzahl.
	 *
	 * @see #operator %
	 *
	 */
	CCDecimal& operator %=(const CCDecimal& op2);

	/** \brief Inkrementiert um 1.
	 *
	 *  Inkrementiert sich selbst um 1. (Siehe #operator +=)
	 *
	 *  @return Referenz auf sich selbst
	 *
	 */
	CCDecimal& operator++();

	/** \brief Dekrementiert um 1.
	 *
	 *  Inkrementiert sich selbst um 1. (Siehe #operator -=)
	 *
	 *  @return Referenz auf sich selbst
	 *
	 */
	CCDecimal& operator--();

	/** \brief Inkrementiert um 1 und liefert alten Wert zurück.
	 *
	 *  Erstellt eine Kopie von sich selbst,
	 *  inkrementiert sich selbst um 1 (Siehe #operator +=)
	 *  und liefert Kopie zurück.
	 *
	 *  @return CCDecimal, Wert vor dem Inkrementieren
	 *
	 */
	CCDecimal operator++(int);

	/** \brief Dekrementiert um 1 und liefert alten Wert zurück.
	 *
	 *  Erstellt eine Kopie von sich selbst,
	 *  dekrementiert sich selbst um 1 (Siehe #operator -=)
	 *  und liefert Kopie zurück.
	 *
	 *  @return CCDecimal, Wert vor dem Dekrementieren
	 *
	 */
	CCDecimal operator--(int);

	//### comparison operators #########################
	/** \brief Prüft auf Gleichheit zweier Dezimalzahlen.
	 *
	 * Verwendet ein Operand die gesamte Kapazität #MAX an Dezimalstellen und
	 * hat mehr Nachkommastellen als die globale Präzision #globalPrecision garantiert,
	 * wird diese anhand der LSD (Least Significant Digit) gerundet.
	 *
	 * @param op2 der CCDecimal, mit dem verglichen werden soll
	 * @return bool ('true', wenn Werte gleich sind)
	 *
	 */
	bool operator ==(const CCDecimal& op2) const;

	/** \brief Prüft auf Ungleichheit zweier Dezimalzahlen.
	 * Verwendet #operator ==.
	 *
	 * Verwendet ein Operand die gesamte Kapazität #MAX an Dezimalstellen und
	 * hat mehr Nachkommastellen als die globale Präzision #globalPrecision garantiert,
	 * wird diese anhand der LSD (Least Significant Digit) gerundet.
	 *
	 * @param op2 der CCDecimal, mit dem verglichen werden soll
	 * @return bool ('true', wenn Werte ungleich sind)
	 *
	 */
	bool operator !=(const CCDecimal& op2) const;

	/** \brief Prüft ob Dezimalzahl kleiner ist als 'op2'.
	 *
	 * Verwendet ein Operand die gesamte Kapazität #MAX an Dezimalstellen und
	 * hat mehr Nachkommastellen als die globale Präzision #globalPrecision garantiert,
	 * wird diese anhand der LSD (Least Significant Digit) gerundet.
	 *
	 * @param op2 der CCDecimal, mit dem verglichen werden soll
	 * @return bool ('true', wenn Wert kleiner ist)
	 *
	 */
	bool operator <(const CCDecimal& op2) const;

	/** \brief Prüft ob Dezimalzahl größer ist als 'op2'.
	 *
	 * Verwendet ein Operand die gesamte Kapazität #MAX an Dezimalstellen und
	 * hat mehr Nachkommastellen als die globale Präzision #globalPrecision garantiert,
	 * wird diese anhand der LSD (Least Significant Digit) gerundet.
	 *
	 * @param op2 der CCDecimal, mit dem verglichen werden soll
	 * @return bool ('true', wenn Wert größer ist)
	 *
	 */
	bool operator >(const CCDecimal& op2) const;

	/** \brief Prüft ob Dezimalzahl kleiner oder gleich 'op2' ist.
	 *
	 * Verwendet ein Operand die gesamte Kapazität #MAX an Dezimalstellen und
	 * hat mehr Nachkommastellen als die globale Präzision #globalPrecision garantiert,
	 * wird diese anhand der LSD (Least Significant Digit) gerundet.
	 *
	 * @param op2 der CCDecimal, mit dem verglichen werden soll
	 * @return bool ('true', wenn Wert kleiner oder gleich ist)
	 *
	 */
	bool operator <=(const CCDecimal& op2) const;

	/** \brief Prüft ob Dezimalzahl größer oder gleich 'op2' ist.
	 *
	 * Verwendet ein Operand die gesamte Kapazität #MAX an Dezimalstellen und
	 * hat mehr Nachkommastellen als die globale Präzision #globalPrecision garantiert,
	 * wird diese anhand der LSD (Least Significant Digit) gerundet.
	 *
	 * @param op2 der CCDecimal, mit dem verglichen werden soll
	 * @return bool ('true', wenn Wert größer oder gleich ist)
	 *
	 */
	bool operator >=(const CCDecimal& op2) const;

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

//	CCDecimal pow(int32_t n){
//
//		CCDecimal result;
//		int arr_size = 0; //array to hold the nth to the 1th power
//
//		//get the absolute power
//		if (n < 0) n = -n;
//
//
//		for (int32_t temp = n; temp > 0; temp >>= 1){
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

/** \brief Stream-Operator
 *
 *  Ermöglicht es einen CCDecimal direkt in einen Output-Stream zu verwenden.
 *  An den Stream wird eine String-Repräsentation angehängt,
 *  welche entsprechend der im Output-Stream gesetzten 'fmtflags' ('precision', 'scientific') erzeugt wird.
 *
 *  @see int32_t #CCDecimal::toString(precOut, bool scientific = false)
 *
 *  @param os oStream, der Output-Stream
 *  @param dec der CCDecimal, der an den Stream angefügt werden soll *
 *  @return Referenz auf den Output-Stream
 *
 */
std::ostream& operator<<(std::ostream &os, const CCDecimal& dec);

#endif /* CCDECIMAL_H_ */
