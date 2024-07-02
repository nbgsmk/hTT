/*
 * LineParser.cpp
 *
 *  Created on: Jul 1, 2024
 *      Author: peca
 */

#include <iostream>
#include <cinttypes>
#include <regex>        // regex, sregex_token_iterator

#include "LineParser.h"

using namespace std;

LineParser::LineParser() {
	this->logEnabled = true;
	this->logLevel = 3;
}

LineParser::~LineParser() {

}

vector<uint8_t> LineParser::parse(string input_line) {
	// vektori sluze samo za prikaz na ekranu
    std::vector<string>		splitStr;
    std::vector<int>		resultInt16base;
    std::vector<uint8_t>	resultChars;
    const std::regex rgx("( )+");			// regex pattern splituje na bilo kakav white space
    std::sregex_token_iterator iter(input_line.begin(), input_line.end(), rgx, -1);
    for (std::sregex_token_iterator end; iter != end; ++iter) {
    	/*
    	 * Npr:
    	 * input_line = "5a 4f 54 4a 41 0a 0d"
    	 * regex splituje na vektor stringova { "5a", "4f", "54", "4a", "41" }
    	 * "stoi" = string to int, base 16
    	 * intVal = integer od svakog elementa { 90,   79,   84,   74,   65 }
    	 * hexChar = castujem svaki u 8-bitni broj <-- OVO saljem na serijski port
    	 *
    	 * primer "A  T"
    	 *         41 54 0d 0a
    	 * odgovor OK
    	 *
    	 * primer "A  T  +  C  G  M  M"
    	 *         41 54 2b 43 47 4d 4d 0d 0a
    	 * odgovor SIMCOM_SIM7600E-H
    	 *
    	 * primer "A  T  +  C  S  U  B"
    	 *         41 54 2b 43 53 55 42		0d 0a
    	 * odgovor +CSUB: B04V03
    	 *         +CSUB: MDM9x07_LE20_S_22_V1.03_210527
    	 * paznja! odgovor sadrzi dva reda!
    	 *
    	 */
		int intVal		=			stoi(iter->str(), 0, 16);		// int reprezentacija ukucanog stringa
		uint8_t hexChar	= (uint8_t)	stoi(iter->str(), 0, 16);		// cast to 8-bit, OVO se salje na serijski port

		// vektori sluze samo za prikaz
		splitStr.push_back(iter->str());
		resultInt16base.push_back(intVal);
    	resultChars.push_back(hexChar);
    }
   	logPrint(splitStr, resultInt16base, resultChars);
	return resultChars;
}


void LineParser::setLogState(bool loggingEnable) {
	this->logEnabled = loggingEnable;
}
bool LineParser::getLogState(){
	return this->logEnabled;
}

void LineParser::setLogLevel(int logLevel) {
	this->logLevel = logLevel;
}
int LineParser::getLogLevel() {
	return this->logLevel;
}

void LineParser::logPrint(vector<string> stringovi, vector<int> intovi, vector<uint8_t> charovi) {
    if ( (this->logEnabled) & (this->logLevel > 0) ) {
		cout << "hex str=";
		for (unsigned int i = 0; i < stringovi.size(); ++i) { cout << " " << stringovi.at(i); }
		cout << endl;

		cout << "str2int=";
		for (unsigned int i = 0; i < intovi.size(); ++i) { cout << " " << intovi.at(i); }
		cout << endl;

		cout << "8bitchr=";
		for (unsigned int i = 0; i < charovi.size(); ++i) { cout << " " << charovi.at(i) << " "; }
		cout << endl;
	}
}
