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
			    std::vector<std::string> result;
			    const std::regex rgx("( )+");
			    std::sregex_token_iterator iter(input_line.begin(), input_line.end(), rgx, -1);
			    for (std::sregex_token_iterator end; iter != end; ++iter) {
			        result.push_back(iter->str());
					int h     =           stoi(iter->str(), 0, 16);
					uint8_t x = (uint8_t) stoi(iter->str(), 0, 16);
					cout << "s=" << iter->str() << "\t h=" << h << "\t x=" << x << endl;
					serial.writeChar(h);
					usleep(1000);		// 1mS malo pauze izmedju karaktera
			    }

			    usleep(1000 * 2000);		// dve sekunde pauze izmedju redova

			}
		}
    }

    serial.closeDevice();    // Close the serial device

	return 0;
}
