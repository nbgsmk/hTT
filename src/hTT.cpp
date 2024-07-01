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
#include <string>
#include <cinttypes>
#include <vector>
#include <sstream>
#include <algorithm>    // copy
#include <iterator>     // back_inserter
#include <regex>        // regex, sregex_token_iterator
#include <thread>
#include "SeriaLib.h"
#include "LineParser.h"

#define SERIAL_PORT "/dev/ttyS0"

using namespace std;


void tx(SeriaLib sr){
	LineParser *lp = new LineParser();
	string input_line = "";
    while(true){
    	getline(cin, input_line);
    	cout << input_line << endl;
    	if (input_line.compare(".") == 0) {
			cout << "Ћао!" << endl;
			break;
		} else {
			if(input_line.size() > 0) {
				vector<uint8_t> hexCharovi = lp->parse(input_line);
				for (unsigned int i = 0; i < hexCharovi.size(); ++i) {
					sr.writeChar(hexCharovi.at(i));
				}
			}
		}
    }

}

void rx(SeriaLib sr){
    // buffer za primljene karaktere
    unsigned int rcvSize = 2000;
	while (true) {
		char rcv[rcvSize];
		if (sr.available() > 0) {
			sr.readString(rcv, '\n', rcvSize-1, 5);
//			cout << "odgovor " << rcv << endl;
			cout << rcv;
		} else {
			// cout << "no reply" ;
		}
		usleep(1000 * 5);
	}

}

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

    thread ttx(tx, serial);
    thread rrx(rx, serial);

    ttx.join();
    serial.closeDevice();    // Close the serial device

    return 0;

//    LineParser *lp = new LineParser();
//    string input_line;
//    while(cin){
//    	getline(cin, input_line);
//    	cout << input_line << endl;
//    	if (input_line.compare(".") == 0) {
//			cout << "Ћао!" << endl;
//			break;
//		} else {
//			if(input_line.size() > 0) {
//				vector<uint8_t> rezultat = lp->parse(input_line);
//
//			    // buffer za primljene karaktere
//			    unsigned int rcvSize = 2000;
//			    for (int i = 0; i < 3; ++i) {
//				    char rcv[rcvSize];
//				    if (serial.available() > 0) {
//				    	serial.readString(rcv, '\n', rcvSize, 100);
//						cout << "odgovor " << rcv << endl;
//					}
//				}
//
//			    usleep(1000 * 2000);		// dve sekunde pauze izmedju redova
//
//			}
//		}
//    }
//
//    serial.closeDevice();    // Close the serial device

//	return 0;
}
