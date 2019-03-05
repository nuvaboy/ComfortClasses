/*
 * CCString.h
 *
 *  Created on: 21.01.2019
 *      Author: jan
 */

#ifndef CCSTRING_H_
#define CCSTRING_H_

class CCDecimal;
#include "CCDecimal.h"

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>

/**
 * @brief Vereinfachte String-Klasse.
 */
class CCString {
private:
	/**
	 *
	 * @brief Internes String-Objekt zur Speicherung und Bereitstellung der meisten String-Funktionen
	 */
	std::string internalStr;

	/**
	 * @class SplitIterator
	 */
	class SplitIterator;

public:
	/**
	 * Enthält den größtmöglichen Wert für size_t.
	 *
	 * Als Parameter bedeutet der Wert typischerweise "bis zum Ende des Strings",
	 * als Rückgabewert zeigt er typischerweise an, dass es keine Funde gab, bspw. bei einer Suchfunktion.
	 *
	 * @see  find
	 * @see  findLast
	 */
	static constexpr size_t npos = std::string::npos;

//## constructors/destructors #############################
	/**
	 * @brief Standardkonstruktor
	 */
	CCString() = default;

	/**
	 * @brief Kopierkonstruktor
	 *
	 * @param ccstr  CCString, der zu kopieren ist
	 */
	CCString(const CCString& ccstr) = default;
	//constructors for textual types
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param str  der einzuspeichernde C++-String
	 */
	CCString(const std::string& str);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param cstr  der einzuspeichernde C-String
	 */
	CCString(const char* cstr);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param c  das einzuspeichernde Zeichen
	 */
	CCString(char c);
//## constructor for boolean type #########################
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * Speichert die Textrepräsentation (typischerweise @p "true" oder @p "false" bzw. @p false) eines Wahrheitswerts ab.
	 *
	 * @param b  der einzuspeichernde Wahrheitswert
	 */
	CCString(bool b);
//## constructors for numeric types #######################
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Ganzzahl
	 */
	CCString(int16_t number);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Ganzzahl
	 */
	CCString(int32_t number);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Ganzzahl
	 */
	CCString(int64_t number);

	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Ganzzahl
	 */
	CCString(uint16_t number);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Ganzzahl
	 */
	CCString(uint32_t number);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Ganzzahl
	 */
	CCString(uint64_t number);

	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Gleitkommazahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 */
	CCString(float number, bool hiPrec = false);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Gleitkommazahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 */
	CCString(double number, bool hiPrec = false);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * @param number  die einzuspeichernde Gleitkommazahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation., ansonsten mit genau 6 Nachkommastellen.
	 */
	CCString(long double number, bool hiPrec = false);

	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * Erzeugt einen CCString aus der Textrepräsentation der gegebenen CCDecimal-Zahl.
	 * Die Zahl wird in wissenschaftlicher Notation repräsentiert und mit der gegebenen
	 * Anzahl Stellen übernommen.
	 *
	 * @param number     die einzuspeichernde CCDecimal-Gleitkommazahl
	 * @param sigDigits  Falls @p true, speichert die Gleitkommazahl mit dieser Anzahl Dezimalstellen.
	 *
	 * @see  CCDecimal::toString()
	 */
	CCString(const CCDecimal& number, int32_t sigDigits);
	/**
	 * @brief Umwandlungskonstruktor
	 *
	 * Erzeugt einen CCString aus der Textrepräsentation der gegebenen CCDecimal-Zahl.
	 * Die Zahl wird mit der über die Präzision der Zahl festgelegten Anzahl Nachkommastellen übernommen.
	 *
	 * @param number  die einzuspeichernde CCDecimal-Gleitkommazahl
	 *
	 * @see  CCDecimal::toString()
	 * @see  CCDecimal::getPrecision()
	 */
	CCString(const CCDecimal& number);

	/**
	 * @brief Fängt ungültige (nicht unterstützte) Typen ab.
	 */
	template<typename type>
	/*  can't convert to CCString    */CCString(type) = delete;

	virtual ~CCString();

	/**
	 * @brief  Gibt eine Kopie des Inhalts als C++-Standardstring zurück.
	 *
	 * @return  Inhalt als C++-String
	 */
	std::string toString() const;

	/**
	 * @brief Umwandlungsoperator. Wandelt einen CCString explizit in einen nullterminierten C-String um.
	 *
	 * "Explizit" bedeutet hier, die Umwandlung findet nur bei Direkt-Initialisierung oder beim Casting statt.
	 * @code
	 * CCString ccstr;
	 *
	 * std::string str(ccstr);           // Direkt-Initialisierung
	 *
	 * printf("%s", (const char*)ccstr); // Casting
	 * @endcode
	 *
	 * @see  operator std::string()
	 */
	explicit operator const char*();
	/**
	 * @brief Umwandlungsoperator. Wandelt einen CCString explizit in einen Standard-C++-String um.
	 *
	 * "Explizit" bedeutet hier, die Umwandlung findet nur bei Direkt-Initialisierung oder beim Casting statt.
	 * @code
	 * CCString ccstr;
	 *
	 * std::string str(ccstr);           // Direkt-Initialisierung
	 *
	 * printf("%s", (const char*)ccstr); // Casting
	 * @endcode
	 *
	 * @see  #operator const char*()
	 */
	explicit operator std::string();

	/**
	 * @brief Streamoperator zum Einfügen in C++-Output-Streams.
	 *
	 * @param os    Output-Stream, in den einzufügen ist.
	 * @param ccstr CCString, von dem zu lesen ist.
	 * @return		Referenz auf os
	 */
	friend std::ostream& operator<<(std::ostream& os, const CCString& ccstr);
	/**
	 * @brief Streamoperator zum Einlesen von C++-Input-Streams.
	 *
	 * @param in	Input-Stream, von dem zu lesen ist.
	 * @param ccstr CCString, in den einzufügen ist.
	 * @return		Referenz auf in
	 */
	friend std::istream& operator>>(std::istream& in, CCString& ccstr);

	/**
	 * @brief Gibt die Länge des Strings zurück.
	 *
	 * @return  die Länge des Strings
	 */
	size_t length() const;

	/**
	 * @brief Gibt eine Referenz auf das Zeichen an Stelle @p index zurück.
	 *
	 * @param index  die Position des Zeichens
	 * @return       eine Referenz auf das Zeichen an der Stelle @p index
	 */
	char& at(size_t index);
	/**
	 * @brief Gibt eine Referenz auf das Zeichen an Stelle @p index zurück.
	 *
	 * @param index  die Position des Zeichens
	 * @return       eine Referenz auf das Zeichen an der Stelle @p index
	 */
	char& operator[](size_t index);
	/**
	 * @brief Gibt eine konstante Referenz auf das Zeichen an Stelle @p index zurück.
	 *
	 * Das Zeichen kann nicht geändert werden.
	 *
	 * @param index  die Position des Zeichens
	 * @return       eine Referenz auf das Zeichen an der Stelle @p index
	 */
	const char& at(size_t index) const;
	/**
	 * @brief Gibt eine konstante Referenz auf das Zeichen an Stelle @p index zurück.
	 *
	 * Das Zeichen kann nicht geändert werden.
	 *
	 * @param index  die Position des Zeichens
	 * @return       eine Referenz auf das Zeichen an der Stelle @p index
	 */
	const char& operator[](size_t index) const;

	/**
	 * @brief Prüft, ob dieser CCString und @p other gleich sind.
	 *
	 * @param other  das andere CCString-Objekt
	 * @return       @p true, wenn die CCStrings gleich sind, sonst @p false.
	 */
	bool operator==(const CCString& other) const;
	/**
	 * @brief Prüft, ob dieser String lexikalisch niederwertiger ist als @p other.
	 *
	 * "Niederwertiger" bedeutet:
	 * - Alle Zeichen von String, die verglichen sind, sind gleich, aber dieser String ist kürzer, oder
	 * - das erste Zeichen, das sich unterscheidet, ist in der Wertigkeit geringer
	 *   (entsprechend des Zahlenwerts des Zeichens).
	 *
	 * @param other  das andere CCString-Objekt
	 * @return       @p true, wenn dieser CCString lexikalisch niederwertiger ist als @p other.
	 */
	bool operator<(const CCString& other) const;

	/**
	 * @brief Fügt einen anderen CCString an diesen an.
	 *
	 * @param ccStr  die anzufügende Zeichenkette
	 * @return       eine Referenz auf dieses Objekt
	 *
	 * @throws length_error falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc    falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(const CCString& ccStr);
	/**
	 * @brief Fügt einen C++-String an diesen CCString an.
	 *
	 * @param str  der anzufügende C++-String
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @throws length_error falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc    falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(const std::string& str);
	/**
	 * @brief Fügt einen C-String an diesen CCString an.
	 *
	 * @param cstr  der anzufügende C-String
	 * @return      eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(const char* cstr);
	/**
	 * @brief Fügt ein Zeichen an diesen CCString an.
	 *
	 * @param c  das anzufügende Zeichen
	 * @return   eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(char c);
	/**
	 * @brief Fügt die Textrepräsentation eines Wahrheitswerts (typischerweise @p "true" oder @p "false") an diesen CCString an.
	 *
	 * @param b  der anzufügende Wahrheitswert
	 * @return   eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(bool b);

	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(int16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(int32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(int64_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(uint16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(uint32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(uint64_t number);

	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(float number, bool hiPrec = false);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(double number, bool hiPrec = false);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& append(long double number, bool hiPrec = false);

	/**
	 * @brief Fängt ungültige (nicht unterstützte) Typen für #append ab.
	 */
	template<typename type>
	/* operand type not supported   */CCString& append(const type&) = delete;

	/**
	 * @brief Fügt einen anderen CCString an diesen an.
	 *
	 * @param ccStr  die anzufügende Zeichenkette
	 * @return       eine Referenz auf dieses Objekt
	 *
	 * @throws length_error falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc    falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(const CCString& ccStr);
	/**
	 * @brief Fügt einen C++-String an diesen CCString an.
	 *
	 * @param str  der anzufügende C++-String
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @throws length_error falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc    falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(const std::string& str);
	/**
	 * @brief Fügt einen C-String an diesen CCString an.
	 *
	 * @param cstr  der anzufügende C-String
	 * @return      eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(const char* cstr);
	/**
	 * @brief Fügt ein Zeichen an diesen CCString an.
	 *
	 * @param c  das anzufügende Zeichen
	 * @return   eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(char c);
	/**
	 * @brief Fügt die Textrepräsentation eines Wahrheitswerts (typischerweise @p "true" oder @p "false") an diesen CCString an.
	 *
	 * @param b  der anzufügende Wahrheitswert
	 * @return   eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(bool b);

	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(int16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(int32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(int64_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(uint16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(uint32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(uint64_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(float number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(double number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator+=(long double number);

	/**
	 * @brief Fängt ungültige (nicht unterstützte) Typen für #operator+= ab.
	 */
	template<typename type>
	/* operand type not supported   */CCString& operator+=(const type&) = delete;

	/**
	 * @brief Fügt einen anderen CCString an diesen an.
	 *
	 * @param ccStr  die anzufügende Zeichenkette
	 * @return       eine Referenz auf dieses Objekt
	 *
	 * @throws length_error falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc    falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(const CCString& ccStr);
	/**
	 * @brief Fügt einen C++-String an diesen CCString an.
	 *
	 * @param str  der anzufügende C++-String
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @throws length_error falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc    falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(const std::string& str);
	/**
	 * @brief Fügt einen C-String an diesen CCString an.
	 *
	 * @param cstr  der anzufügende C-String
	 * @return      eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(const char* cstr);
	/**
	 * @brief Fügt ein Zeichen an diesen CCString an.
	 *
	 * @param c  das anzufügende Zeichen
	 * @return   eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(char c);
	/**
	 * @brief Fügt die Textrepräsentation eines Wahrheitswerts (typischerweise @p "true" oder @p "false") an diesen CCString an.
	 *
	 * @param b  der anzufügende Wahrheitswert
	 * @return   eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(bool b);

	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(int16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(int32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(int64_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(uint16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(uint32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(uint64_t number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(float number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(double number);
	/**
	 * @brief Fügt eine Zahl in Textform an diesen CCString an.
	 *
	 * @param number  die anzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 */
	CCString& operator<<(long double number);

	/**
	 * @brief Fängt ungültige (nicht unterstützte) Typen für #operator<< ab.
	 */
	template<typename type>
	/* operand type not supported   */CCString& operator<<(const type&) = delete;

	/**
	 * @brief Überschreibt einen Teil dieses Strings durch den gegebenen CCString.
	 *
	 * Ersetzt alle Zeichen von @p pos an durch @p ccstr, maximal jedoch bis zum Ende dieses Strings
	 *   @n Hinweis: arithmetische Typen werden von replace nicht unterstützt.
	 *      Kapselung in einem CCString-Objekt wird empfohlen, z.B.:
	 *      @code
	 *      CCString ccstr1("#################");
	 *      double d = 1.23456789;
	 *
	 *      CCString ccstr2 = ccstr1.replace(3, CCString(d));
	 *      @endcode
	 * @param pos    die Stelle im String, an der eingefügt werden soll
	 *               @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param ccstr  der hineinzukopierende String
	 * @return       eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& replaceAt(size_t pos, const CCString& ccstr);
	/**
	 * @brief Überschreibt einen Teil dieses Strings durch den gegebenen C++-SString.
	 *
	 * Ersetzt alle Zeichen von @p pos an durch @p ccstr, maximal jedoch bis zum Ende dieses Strings.
	 * @param pos  die Stelle im String, an der eingefügt werden soll
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param str  der hineinzukopierende String
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 *
	 * @see  #replaceAt(size_t,const CCString&)
	 */
	CCString& replaceAt(size_t pos, const std::string& str);
	/**
	 * @brief Überschreibt einen Teil dieses Strings durch den gegebenen C-String.
	 *
	 * Ersetzt alle Zeichen von @p pos an durch @p cstr, maximal jedoch bis zum Ende dieses Strings.
	 * @param pos   die Stelle im String, an der eingefügt werden soll
	 *              @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param cstr  der hineinzukopierende String
	 * @return      eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 *
	 * @see  #replaceAt(size_t,const CCString&)
	 */
	CCString& replaceAt(size_t pos, const char* cstr);
	/**
	 * @brief Überschreibt das Zeichen and der Stelle @p pos mit @c c.
	 *
	 * @param pos  die Stelle, an der überschrieben werden soll
	 * @param c    das Zeichen, das geschrieben werden soll
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @see  #replaceAt(size_t,const CCString&)
	 */
	CCString& replaceAt(size_t pos, char c);
	/**
	 * @brief Fängt ungültige (nicht unterstützte) Typen für replaceAt ab.
	 *
	 * @see  #replaceAt(size_t,const CCString&)
	 */
	template<typename type>
	/* operand type not supported   */CCString& replaceAt(size_t pos, const type&) = delete;

	/**
	 * @brief Fügt einen anderen CCString in diesen ein.
	 *
	 * @param pos    die Stelle im String, an der eingefügt werden soll
	 *               @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param ccStr  die anzufügende Zeichenkette
	 * @return       eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, const CCString& ccStr);
	/**
	 * @brief Fügt einen C++-String in diesen CCString ein.
	 *
	 * @param pos  die Stelle im String, an der eingefügt werden soll
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param str  der anzufügende C++-String
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, const std::string& str);
	/**
	 * @brief Fügt einen C-String in diesen CCString ein.
	 *
	 * @param pos   die Stelle im String, an der eingefügt werden soll
	 *              @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param cstr  der anzufügende C-String
	 * @return      eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, const char* cstr);
	/**
	 * @brief Fügt ein Zeichen in diesen CCString ein.
	 *
	 * @param pos  die Stelle im String, an der eingefügt werden soll
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param c    das anzufügende Zeichen
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, char c);
	/**
	 * @brief Fügt die Textrepräsentation eines Wahrheitswerts (typischerweise @p "true" oder @p "false") in diesen CCString ein.
	 *
	 * @param pos  die Stelle im String, an der eingefügt werden soll
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param b    der anzufügende Wahrheitswert
	 * @return     eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, bool b);

	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, int16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, int32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, int64_t number);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, uint16_t number);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, uint32_t number);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, uint64_t number);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, float number, bool hiPrec = false);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, double number, bool hiPrec = false);
	/**
	 * @brief Fügt eine Zahl in Textform in den CCString ein.
	 *
	 * @param pos     die Stelle im String, an der eingefügt werden soll
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param number  die einzufügende Zahl
	 * @param hiPrec  Falls @p true, speichert die Gleitkommazahl mit maximaler Anzahl Dezimalstellen ab,
	 *                ggf. in wissenschaftlicher Notation, ansonsten mit genau 6 Nachkommastellen.
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws length_error  falls der CCString nach dieser Operation die Maximallänge eines Strings überschreitet.
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& insert(size_t pos, long double number, bool hiPrec = false);

	/**
	 * @brief Fängt ungültige (nicht unterstützte) Typen für #insert ab.
	 */
	template<typename type>
	/* operand type not supported   */CCString& insert(size_t pos, const type&) = delete;

	/**
	 * @brief Löscht Zeichen aus dem CCString.
	 *
	 * Entfernt Zeichen beginnend an der Stelle @p pos, bis @p length Zeichen gelöscht sind,
	 * maximal jedoch bis zum Ende des Strings.
	 *
	 * @param pos     Position des ersten zu entfernenden Zeichens
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param length  Anzahl der zu entfernenden Zeichen
	 * @return        eine Referenz auf dieses Objekt
	 *
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString& erase(size_t pos, size_t length);

	/**
	 * @brief Entfernt nicht druckbare Zeichen und Leerzeichen von Anfang und Ende des Strings.
	 *
	 * @return  eine Referenz auf dieses Objekt
	 */
	CCString& trim();

	/**
	 * @brief Erzeugt einen Teilstring.
	 *
	 * Erzeugt einen neuen CCString und kopiert @p length Zeichen dieses Strings von @p pos an.
	 *
	 * @param pos     der Index des ersten zu kopierenden Zeichens
	 *                @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @param length  die Anzahl der zu kopierenden Zeichen
	 * @return        der erzeugte Teilstring
	 *
	 * @throws bad_alloc     falls dem String intern kein Speicher zugewiesen werden konnte.
	 * @throws out_of_range  falls @p pos nicht innerhalb des Strings liegt.
	 */
	CCString subString(size_t pos, size_t length) const;

	/**
	 * @brief Sucht nach dem ersten Auftreten des Strings @p ccstr im aktuellen String.
	 *
	 * @param ccstr  Der String, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 * @param pos    Die Stelle, von der an zu suchen ist. Jedes Zeichen davor wird ignoriert.
	 *               Standardmäßig wird der gesamte String durchsucht.
	 *               @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return       Der Index des ersten Zeichens des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t find(const CCString& ccstr, size_t pos = 0) const;
	/**
	 * @brief Sucht nach dem ersten Auftreten des Strings @p str im aktuellen String.
	 *
	 * @param str  Der String, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 * @param pos  Die Stelle, von der an zu suchen ist. Jedes Zeichen davor wird ignoriert.
	 *             Standardmäßig wird der gesamte String durchsucht.
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return     Der Index des ersten Zeichens des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t find(const std::string& str, size_t pos = 0) const;
	/**
	 * @brief Sucht nach dem ersten Auftreten des Strings @p cstr im aktuellen String.
	 *
	 * @param cstr  Der String, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 *              @n Hinweis: Der C-String muss null-terminiert sein!
	 * @param pos   Die Stelle, von der an zu suchen ist. Jedes Zeichen davor wird ignoriert.
	 *              Standardmäßig wird der gesamte String durchsucht.
	 *              @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return      Der Index des ersten Zeichens des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t find(const char* cstr, size_t pos = 0) const;
	/**
	 * @brief Sucht nach dem ersten Auftreten des Zeichens @p c im aktuellen String.
	 *
	 * @param c    Das Zeichen, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 * @param pos  Die Stelle, von der an zu suchen ist. Jedes Zeichen davor wird ignoriert.
	 *             Standardmäßig wird der gesamte String durchsucht.
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return     Der Index des ersten Zeichens des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t find(char c, size_t pos = 0) const;

	/**
	 * @brief Sucht nach dem letzten Auftreten des Strings @p ccstr im aktuellen String.
	 *
	 * @param ccstr  Der String, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 * @param pos    Die Stelle des letzten Zeichens, das der Beginn eines Fundes sein kann.
	 *               Jedes Zeichen danach wird als Beginn eines Fundes ignoriert.
	 *               Standardmäßig wird der gesamte String durchsucht.
	 *               @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return       Der Index des ersten Zeichens des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t findLast(const CCString& ccstr, size_t pos = npos) const;
	/**
	 * @brief Sucht nach dem letzten Auftreten des Strings @p str im aktuellen String.
	 *
	 * @param str  Der String, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 * @param pos  Die Stelle des letzten Zeichens, das der Beginn eines Fundes sein kann.
	 *             Jedes Zeichen danach wird als Beginn eines Fundes ignoriert.
	 *             Standardmäßig wird der gesamte String durchsucht.
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return     Der Index des ersten Zeichens des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t findLast(const std::string& str, size_t pos = npos) const;
	/**
	 * @brief Sucht nach dem letzten Auftreten des Strings @p cstr im aktuellen String.
	 *
	 * @param cstr  Der String, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 * @param pos   Die Stelle des letzten Zeichens, das der Beginn eines Fundes sein kann.
	 *              Jedes Zeichen danach wird als Beginn eines Fundes ignoriert.
	 *              Standardmäßig wird der gesamte String durchsucht.
	 *              @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return      Der Index des ersten Zeichens des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t findLast(const char* cstr, size_t pos = npos) const;
	/**
	 * @brief Sucht nach dem letzten Auftreten des Zeichens @p c im aktuellen String.
	 *
	 * @param c    Der String, nach dem zu suchen ist. Jedes Zeichen muss übereinstimmen.
	 * @param pos  Die Stelle des letzten Zeichens, das ein Fund sein kann.
	 *             Jedes Zeichen danach wird als Fund ignoriert.
	 *             Standardmäßig wird der gesamte String durchsucht.
	 *             @n Hinweis: Das erste Zeichen hat den Index 0.
	 * @return     Der Index des Fundes oder #npos, falls nichts gefunden wurde.
	 */
	size_t findLast(char c, size_t pos = npos) const;

	/**
	 * @brief Erzeugt einen SplitIterator, der den CCString anhand des gegebenen Trennzeichens auftrennt.
	 *
	 * Das "Trennzeichen" kann ein einzelnes Zeichen sein, ein String oder ein regulärer Ausdruck,
	 * spezifiziert als regulärer Ausdruck.
	 * Der erzeugte Iterator zeigt auf den allerersten Teilstring.
	 *
	 * Wird dieser Iterator inkrementiert, zeigt der Iterator auf den nächsten Teilstring. Wird der
	 * Iterator nach dem letztmöglichen Fund inkrementiert, wird er äquivalent zu #splitEnd.
	 *
	 * @param regex  das Trennzeichen, spezifiziert als regulärer Ausdruck
	 * @return       ein SplitIterator, der auf den ersten Teilstring zeigt
	 *
	 * @see  SplitIterator::SplitIterator(const CCString&, const CCString&)
	 */
	SplitIterator splitBegin(const CCString& regex) const;
	/**
	 * @brief Erzeugt einen SplitIterator, der das Ende einer Auftrennung markiert.
	 *
	 * Dieser Iterator zeigt hinter den letzten aller möglichen Teilstrings,
	 * unabhängig eines Trennzeichens.
	 *
	 * @return ein SplitIterator, der auf den Teilstring hinter jedem möglichem Teilstring zeigt.
	 *
	 * @see  SplitIterator::SplitIterator(const CCString&)
	 */
	SplitIterator splitEnd() const;

	/**
	 * @brief Prüft, ob dieser String dem regulären Ausdruck @p regex genügt.
	 *
	 * Hinweis: @n Reguläre Ausdrücke verwenden die C++-Variante der ECMAScript-Grammatik für reguläre Ausdrücke
	 * (<a href="http://ecma-international.org/ecma-262/5.1/#sec-15.10">ECMAScript RegEx-Grammatik (Standard)</a>,
	 * <a href="https://en.cppreference.com/w/cpp/regex/ecmascript">ECMAScript RegEx-Grammatik (modifiziert, C++-Variante)</a>).
	 *
	 * @param regex  der reguläre Ausdruck, dem dieser String entsprechen soll
	 * @return       @p true, wenn der String diesem regulären Ausdruck entspricht, sonst @p false
	 */
	bool isMatch(const CCString& regex) const;
	/**
	 * @brief Prüft, ob ein Teil dieses Strings dem regulären Ausdruck @p regex entspricht.
	 *
	 * Hinweis: @n Reguläre Ausdrücke verwenden die C++-Variante der ECMAScript-Grammatik für reguläre Ausdrücke
	 * (<a href="http://ecma-international.org/ecma-262/5.1/#sec-15.10">ECMAScript RegEx-Grammatik (Standard)</a>,
	 * <a href="https://en.cppreference.com/w/cpp/regex/ecmascript">ECMAScript RegEx-Grammatik (modifiziert, C++-Variante)</a>).
	 *
	 * @param regex  der reguläre Ausdruck, nach dem in diesem String gesucht werden soll
	 * @return       @p true, wenn ein Teil dieses Strings diesem regulären Ausdruck entspricht, sonst @p false
	 */
	bool containsMatch(const CCString& regex) const;

	/**
	 * @brief Gibt den Teil dieses Strings zurück, der dem regulären Ausdruck @p regex entspricht.
	 *
	 * Hinweis: @n Reguläre Ausdrücke verwenden die C++-Variante der ECMAScript-Grammatik für reguläre Ausdrücke
	 * (<a href="http://ecma-international.org/ecma-262/5.1/#sec-15.10">ECMAScript RegEx-Grammatik (Standard)</a>,
	 * <a href="https://en.cppreference.com/w/cpp/regex/ecmascript">ECMAScript RegEx-Grammatik (modifiziert, C++-Variante)</a>).
	 *
	 * @param regex  der reguläre Ausdruck, nach dem in diesem String gesucht werden soll
	 * @return       Der Treffer der Suche oder ein leerer String, falls nichts gefunden wurde.
	 *               Ist genau dann gleichwertig zu diesem String, falls <tt>isMatch(regex) == true</tt>.
	 */
	CCString getMatch(const CCString& regex) const;

	/**
	 * @brief Ersetzt alle Vorkommen des regulären Ausdrucks @p regex durch den regulären Ausdruck @p replacement.
	 *
	 * Arbeitet an einer Kopie dieses Strings.
	 *
	 * Hinweis: @n Reguläre Ausdrücke verwenden die C++-Variante der ECMAScript-Grammatik für reguläre Ausdrücke
	 * (<a href="http://ecma-international.org/ecma-262/5.1/#sec-15.10">ECMAScript RegEx-Grammatik (Standard)</a>,
	 * <a href="https://en.cppreference.com/w/cpp/regex/ecmascript">ECMAScript RegEx-Grammatik (modifiziert, C++-Variante)</a>).
	 *
	 * @param regex        der reguläre Ausdruck, nach dem in diesem String gesucht werden soll
	 * @param replacement  der reguläre Ausdruck, mit dem Treffer ersetzt werden sollen
	 * @return             die bearbeitete Kopie dieses Strings.
	 *                     Gleichwertig zu diesem String, falls @p regex nicht gefunden wurde.
	 */
	CCString replaceAll(const CCString& regex, const CCString& replacement) const;
	/**
	 * @brief Ersetzt das erste Vorkommen des regulären Ausdrucks @p regex durch den regulären Ausdruck @p replacement.
	 *
	 * Arbeitet an einer Kopie dieses Strings.
	 *
	 * Hinweis: @n Reguläre Ausdrücke verwenden die C++-Variante der ECMAScript-Grammatik für reguläre Ausdrücke
	 * (<a href="http://ecma-international.org/ecma-262/5.1/#sec-15.10">ECMAScript RegEx-Grammatik (Standard)</a>,
	 * <a href="https://en.cppreference.com/w/cpp/regex/ecmascript">ECMAScript RegEx-Grammatik (modifiziert, C++-Variante)</a>).
	 *
	 * @param regex        der reguläre Ausdruck, nach dem in diesem String gesucht werden soll
	 * @param replacement  der reguläre Ausdruck, mit dem Treffer ersetzt werden sollen
	 * @return             die bearbeitete Kopie dieses Strings.
	 *                     Gleichwertig zu diesem String, falls @p regex nicht gefunden wurde.
	 */
	CCString replaceFirst(const CCString& regex, const CCString& replacement) const;

};

/**
 * @brief Konkatenation zweier CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(const CCString& lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und eines C-Strings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(const char*)
 */
CCString operator+(const CCString& lhs, const char* rhs);
/**
 * @brief Konkatenation eines C-Strings und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const char*)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(const char* lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und eines C++-Strings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(const std::string&)
 */
CCString operator+(const CCString& lhs, const std::string& rhs);
/**
 * @brief Konkatenation eines C++-Strings und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const std::string&)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(const std::string& lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und eines Zeichens.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(char)
 */
CCString operator+(const CCString& lhs, char rhs);
/**
 * @brief Konkatenation eines Zeichens und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(char)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(char lhs, const CCString& rhs);


/**
 * @brief Konkatenation der Textform eines Wahrheitswerts und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(bool)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(bool lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und der Textform eines Wahrheitswerts.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(bool)
 */
CCString operator+(const CCString& lhs, bool rhs);

/**
 * @brief Konkatenation eines CCStrings und einer Ganzzahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(int16_t)
 */
CCString operator+(const CCString& lhs, int16_t rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(int16_t)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(int16_t lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und einer Ganzzahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(int32_t)
 */
CCString operator+(const CCString& lhs, int32_t rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(int32_t)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(int32_t lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und einer Ganzzahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(int64_t)
 */
CCString operator+(const CCString& lhs, int64_t rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(int64_t)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(int64_t lhs, const CCString& rhs);

/**
 * @brief Konkatenation eines CCStrings und einer Ganzzahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(uint16_t)
 */
CCString operator+(const CCString& lhs, uint16_t rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(uint16_t)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(uint16_t lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und einer Ganzzahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(uint32_t)
 */
CCString operator+(const CCString& lhs, uint32_t rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(uint32_t)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(uint32_t lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und einer Ganzzahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(uint64_t)
 */
CCString operator+(const CCString& lhs, uint64_t rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(uint64_t)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(uint64_t lhs, const CCString& rhs);

/**
 * @brief Konkatenation eines CCStrings und einer Gleitkommazahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(float)
 */
CCString operator+(const CCString& lhs, float rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(float)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(float lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und einer Gleitkommazahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(double)
 */
CCString operator+(const CCString& lhs, double rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(double)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(double lhs, const CCString& rhs);
/**
 * @brief Konkatenation eines CCStrings und einer Gleitkommazahl.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(const CCString&)
 * @see    CCString::operator+=(long double)
 */
CCString operator+(const CCString& lhs, long double rhs);
/**
 * @brief Konkatenation einer Ganzzahl und eines CCStrings.
 *
 * Äquivalent zu:
 * @n<tt>CCString(lhs)+=rhs;</tt>
 *
 * @param  lhs
 * @param  rhs
 * @return rhs, angefügt an lhs
 * @see    CCString::CCString(long double)
 * @see    CCString::operator+=(const CCString&)
 */
CCString operator+(long double lhs, const CCString& rhs);

/**
 * @brief Fängt ungültige (nicht unterstützte) Typen für #operator+ ab.
 */
template<typename type1, typename type2>
/* operand type not supported   */CCString operator+(const type1&, const type2&) = delete;

/* as per https://en.cppreference.com/w/cpp/language/operators */
/**
 * @brief Prüft zwei CCStrings auf Ungleichheit
 * @param lhs der erste CCString
 * @param rhs der zweite CCString
 * @return @p true, falls @code!(lhs == rhs)@endcode @p true ergibt, sonst @p false.
 * @see   CCString::operator==()
 */
inline bool operator!=(const CCString& lhs, const CCString& rhs) {
	return !(lhs == rhs);
}
/**
 * @brief Prüft, ob der erste CCString lexikalisch höherwertiger ist als der zweite.
 * @param lhs der erste CCString
 * @param rhs der zweite CCString
 * @return @p true, falls @code rhs < lhs@endcode @p true ergibt, sonst @p false.
 * @see   CCString::operator<()
 */
inline bool operator>(const CCString& lhs, const CCString& rhs) {
	return rhs < lhs;
}
/**
 * @brief Prüft, ob der erste CCString lexikalisch gleich- oder niederwertiger zum zweiten ist.
 * @param lhs der erste CCString
 * @param rhs der zweite CCString
 * @return @p true, falls @code!(lhs > rhs)@endcode @p true ergibt, sonst @p false.
 * @see   CCString::operator<()
 */
inline bool operator<=(const CCString& lhs, const CCString& rhs) {
	return !(lhs > rhs);
}
/**
 * @brief Prüft, ob der erste CCString lexikalisch gleich- oder höherwertiger zum zweiten ist.
 * @param lhs der erste CCString
 * @param rhs der zweite CCString
 * @return @p true, falls @code!(lhs < rhs)@endcode @p true ergibt, sonst @p false.
 * @see   CCString::operator<()
 */
inline bool operator>=(const CCString& lhs, const CCString& rhs) {
	return !(lhs < rhs);
}

/**
 * @class CCString::SplitIterator
 * @brief Iterator-Typ zur Implementierung von Auftrennungen an Trennzeichen.
 *
 * Iteriert über alle Teil-Strings zwischen den Trennzeichen, einschließlich leerer Teilstrings,
 * falls sich Trennzeichen hintereinander befinden oder sich ein Trennzeichen am Anfang oder Ende befindet.
 */
class CCString::SplitIterator: public std::iterator<std::input_iterator_tag, const CCString> {
private:
	/**
	 * @brief Zeiger auf den CCString, der aufgetrennt wird.
	 */
	const CCString* originString;

	/**
	 * @brief Der reguläre Ausdruck, der das Trennzeichen darstellt, als C++-String
	 */
	std::string separatorRegex;

	/**
	 * @brief Zeiger auf den aktuellen Teilstring
	 */
	std::unique_ptr<CCString> currentSplit;

	/**
	 * @brief Der Reststring, der noch aufzutrennen ist.
	 */
	std::string currentRemainder;

	/**
	 * @brief Zeigt an, ob es beim letzten Aufruf von doSplit einen Match gab.
	 *
	 * Ist standardmäßig @p true, damit auch Strings ohne Trennzeichen wiedergegeben werden.
	 */
	bool hadMatch = true;

	/**
	 * @brief Zeigt an, ob die String-Verarbeitung abgeschlossen ist.
	 *
	 * Abgeschlossen heißt, der Iterator zeigt jetzt hinter den letzten Teilstring und ist somit äquivalent zu splitEnd().
	 */
	bool isFinished = false;

	/**
	 * @brief Trennt den nächsten Teilstring ab.
	 *
	 * Sucht das nächste Trennzeichen in #currentRemainder und speichert alle Zeichen
	 * vor dem Trennzeichen in #currentSplit ab und alle Zeichen danach in #currentRemainder.
	 * Aktualisiert hadMatch und isFinished entsprechend.
	 */
	void doSplit();
public:

	/**
	 * @brief Kopierkonstruktor
	 *
	 * Erzeugt eine vollständige Kopie des Iterators. Alle internen Variablen werden kopiert.
	 * @param orig Zu kopierender SplitIterator
	 */
	SplitIterator(const SplitIterator& orig);

	/**
	 * @brief Erzeugt einen SplitIterator, der @p origin anhand des Trennzeichens @p regex auftrennt.
	 *
	 * Dieser Konstruktor wird ausschließlich von #splitBegin() aufgerufen.
	 *
	 * Nach der Member-Initialisierung wird eine Trennung durchgeführt, damit der Iterator auf
	 * den ersten Teilstring zeigt.
	 *
	 * @param origin der CCString, auf dem der Iterator arbeitet
	 * @param regex  der reguläre Ausdruck (als CCString), der das Trennzeichen angibt
	 * @see   CCString::splitBegin()
	 */
	SplitIterator(const CCString& origin, const CCString& regex);

	/**
	 * @brief Erzeugt einen SplitIterator, der hinter den letzten Teilstring zeigt.
	 *
	 * Dieser Konstruktor wird ausschließlich von #splitEnd() aufgerufen.
	 *
	 * Der erzeugte Iterator markiert das Ende einer Auftrennung des CCStrings @p origin.
	 * Er benötigt kein Trennzeichen, da er bereits auf den Teilstring hinter jedem möglichen
	 * Teilstring zeigt.
	 *
	 * Intern sind alle Strings als leer initialisiert und isFinished ist als @p true gesetzt.
	 *
	 * @param origin der CCString, auf dem der Iterator arbeitet
	 * @see   CCString::splitEnd()
	 */
	SplitIterator(const CCString& origin);

	/**
	 * @brief  Prefix-Inkrement. Bewegt den Iterator weiter zum nächsten Teilstring.
	 * @return Referenz auf denselben Iterator
	 */
	SplitIterator& operator++();

	/**
	 * @brief  Postfix-Inkrement. Bewegt den Iterator weiter zum nächsten Teilstring.
	 * @return Kopie des Iterators im vorherigen Zustand
	 */
	SplitIterator operator++(int);

	/**
	 * @brief Ermöglicht Dereferenzierung zu aktuellem Teilstring.
	 * @return konstante Referenz auf den aktuellen Teilstring
	 */
	const CCString& operator*() const;

	/**
	 * @brief Ermöglicht Zugriff auf Member des aktuellen Teilstrings.
	 * @code
	 * //SplitIterator splIt
	 * splIt->length();
	 * @endcode
	 * ist äquivalent zu:
	 * @code
	 * (*splIt).length();
	 * @endcode
	 * @return konstanter Zeiger auf den letzten Teilstring
	 */
	const CCString* operator->() const;

	/**
	 * @brief Prüft zwei Iteratoren auf Gleichheit.
	 *
	 * Gleichheit herrscht dann, wenn
	 * - die Iteratoren auf demselben CCString arbeiten,
	 * - die Iteratoren das gleiche Trennzeichen aufweisen und
	 * - die Iteratoren den gleichen Teilstring und Reststring haben.
	 *
	 * Iteratoren, die hinter den letzten Teilstring zeigen, werden separat behandelt.
	 * @param other
	 * @return @p true, wenn die Iteratoren gleich sind
	 */
	bool operator==(const SplitIterator& other) const;

	/**
	 * @brief Prüft zwei Iteratoren auf Ungleichheit.
	 * @param other der andere Iterator
	 * @return @p true, wenn @code!(*this == other)@endcode @p true ergibt, sonst @p false.
	 * @see   SplitIterator::operator==
	 */
	bool operator!=(const SplitIterator& other) const {
		return !(operator==(other));
	}
};

#endif /* CCSTRING_H_ */
