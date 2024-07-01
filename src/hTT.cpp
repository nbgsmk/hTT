//============================================================================
// Name        : TT.cpp
// Author      : z
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <regex>        // regex, sregex_token_iterator
#include <thread>
#include "SeriaLib.h"
#include "LineParser.h"

using namespace std;

enum TXmode {
	modeHEX,
	modeASCII
};


//const char* SERIAL_PORT = "/dev/ttyS0";
const char* SERIAL_PORT = "/dev/ttyUSB4";
const unsigned int BAUD = 115200;
TXmode txMode = modeHEX;

void tx(SeriaLib sr, TXmode mode, bool isRedirekcija) {
	LineParser *lp = new LineParser();
	while ( cin.eof() == false ) {		// u slucaju redirekcije ulaza, kad se potrosi ulazni fajl, izlazim
		string input_line = "";
		getline(cin, input_line);
		if (input_line.compare(".") == 0) {
			cout << "Ћао!" << endl;
			break;
		} else {
			if (input_line.size() > 0) {
				switch (mode) {
					case modeHEX:{
						vector<uint8_t> hexCharovi = lp->parse(input_line);
						for (unsigned int i = 0; i < hexCharovi.size(); ++i) {
							sr.writeChar(hexCharovi.at(i));
						}
						break;
					}
					case modeASCII: {
						if (isRedirekcija) {
							cout << input_line << endl;
						}
						const char *cs = input_line.c_str();
						sr.writeString(cs);
						sr.writeString("\r");
						break;
					}
				}
				this_thread::yield();
				if (isRedirekcija) {
					this_thread::sleep_for(5000ms);
				}
			}
		}
	}

}

void rx(SeriaLib sr) {
	// buffer za primljene karaktere
	unsigned int rcvSize = 2000;
	while (true) {
		char rcv[rcvSize];
		if (sr.available() > 0) {
			sr.readString(rcv, '\n', rcvSize - 1, 5);
			// cout << "odgovor " << rcv << endl;
			cout << rcv;
		} else {
			this_thread::yield();
		}
	}
}

int main() {
	cout << "Hey" << endl; // prints Hey

	// Serial object
	SeriaLib serial;

	// Connection to serial port
	char errorOpening = serial.openDevice(SERIAL_PORT, BAUD);

	// If connection fails, return the error code otherwise, display a success message
	if (errorOpening != 1) {
		cout << "Error connecting to " << SERIAL_PORT << endl;
		return errorOpening;
	} else {
		cout << "Connected to " << SERIAL_PORT << " at " << BAUD << endl;
	}

	switch (txMode) {
		case modeASCII:{
			cout << "Terminal mode is ASCII" << endl;
			break;
		}
		case modeHEX:{
			cout << "Terminal mode is HEX" << endl;
			cout << "Unesi HEX karaktere, npr ovako 5a 4f 54 4a 41" << endl;
			break;
		}
	}
	bool stdinRedirekcija = false;
	if ( isatty(STDIN_FILENO) ) {
		stdinRedirekcija = false;
		cout << "Za kraj unesi SAMO TACKU u novom redu i <enter>" << endl;
	} else {
		stdinRedirekcija = true;
		cout << "Citam komande iz fajla (redirekcija)" << endl;
	}
	cout << endl;

	thread ttx(tx, serial, txMode, stdinRedirekcija);
	thread rrx(rx, serial);

	ttx.join();
	serial.closeDevice();    // Close the serial device

	return 0;

}
