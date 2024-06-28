//============================================================================
// Name        : TT.cpp
// Author      : z
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <cinttypes>
#include <vector>
#include <sstream>
#include <algorithm>    // copy
#include <iterator>     // back_inserter
#include <regex>        // regex, sregex_token_iterator
#include "SeriaLib.h"

#define SERIAL_PORT "/dev/ttyS0"

using namespace std;


int main() {
	cout << "Hey" << endl; // prints Hey

    // Serial object
    SeriaLib serial;

    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 115200);


    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening!=1) {
    	cout << "Error connecting to " << SERIAL_PORT << endl;
    	return errorOpening;
    } else {
    	cout << "Connected to " << SERIAL_PORT << endl;
    }

    cout << "Unesi HEX znakove, red po red" << endl;
    cout << "Za kraj unesi samo tacku na pocetku reda, npr ovako" << endl;
    cout << "." << endl;
    cout << endl;

    string input_line;
    while(cin){
    	getline(cin, input_line);
    	cout << input_line << endl;
    	if (input_line.compare(".") == 0) {
			cout << "Ћао!" << endl;
			break;
		} else {
			if(input_line.size() > 0) {
				// vektori sluze samo za prikaz na ekranu
			    std::vector<string>		splitStr;
			    std::vector<int>		resultHex;
			    std::vector<uint8_t>	resultChar;
			    const std::regex rgx("( )+");			// regex pattern splituje na bilo kakav white space
			    std::sregex_token_iterator iter(input_line.begin(), input_line.end(), rgx, -1);
			    for (std::sregex_token_iterator end; iter != end; ++iter) {
			    	/*
			    	 * Npr:
			    	 * input_line = "5a 4f 54 4a 41"
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
			    	 *         41 54 2b 43 47 4d 4d		0d 0a
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
			    	resultHex.push_back(intVal);
			    	resultChar.push_back(hexChar);
					serial.writeChar(hexChar);
					// cout << "s=" << iter->str() << "\t h=" << intVal << "\t x=" << hexChar << endl;
					usleep(1000);		// malo pauze izmedju karaktera (1mS)
			    }
			    cout << "str=";
			    for (unsigned int i = 0; i < splitStr.size(); ++i) { cout << "\t" << splitStr.at(i); }
			    cout << endl;

			    cout << "hex=";
			    for (unsigned int i = 0; i < splitStr.size(); ++i) { cout << "\t" << resultHex.at(i); }
			    cout << endl;

			    cout << "chr=";
			    for (unsigned int i = 0; i < splitStr.size(); ++i) { cout << "\t" << resultChar.at(i); }
			    cout << endl;


			    // buffer za primljene karaktere
			    unsigned int rcvSize = 2000;
			    for (int i = 0; i < 3; ++i) {
				    char rcv[rcvSize];
				    if (serial.available() > 0) {
				    	serial.readString(rcv, '\n', rcvSize, 100);
						cout << "odgovor " << rcv << endl;
					}
				}

			    usleep(1000 * 2000);		// dve sekunde pauze izmedju redova

			}
		}
    }

    serial.closeDevice();    // Close the serial device

	return 0;
}
