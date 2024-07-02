//============================================================================
// Name        : TT.cpp
// Author      : z
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <regex>        // regex, sregex_token_iterator
#include <thread>
#include "SeriaLib.h"
#include "LineParser.h"

using namespace std;

enum TerminalMode {
	modeHEX,
	modeASCII
};


const char* SERIAL_PORT = "/dev/ttyS0";
//const char* SERIAL_PORT = "/dev/ttyUSB4";
const char* SERIAL_BAUD = "115200";
TerminalMode txMode = modeHEX;
TerminalMode rxMode = txMode;

void tx(SeriaLib sr, TerminalMode txMod, bool isRedirekcija) {
	LineParser *lp = new LineParser();
	while ( cin.eof() == false ) {		// u slucaju redirekcije ulaza, kad se potrosi ulazni fajl, izlazim
		string input_line = "";
		getline(cin, input_line);
		if (input_line.compare(".") == 0) {
			cout << "Ћао!" << endl;
			break;
		} else {
			if (input_line.size() > 0) {
				switch (txMod) {
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
						input_line += "\r\n";
						const char *cs = input_line.c_str();
						sr.writeString(cs);
						break;
					}
				}
				this_thread::yield();
				if (isRedirekcija) {
					for (int i = 0; i < 500; ++i) {
						this_thread::yield();
						this_thread::sleep_for(10ms);

					}
				}
			}
		}
	}

}

void rx(SeriaLib sr, TerminalMode rxMod) {
	// buffer za primljene karaktere
	unsigned int rcvSize = 1000;
	while (true) {
		char rcv[rcvSize];
		unsigned int avail = sr.available();
		if (avail > 0) {
			 sr.readBytes(rcv, rcvSize, 20, 200);
//			sr.readString(rcv, '\r', rcvSize - 1, 5);
			switch (rxMod) {
				case modeASCII:{
					cout << rcv;
					break;
				}

				case modeHEX:{
					stringstream  hs;
					stringstream  chs;
					for (unsigned int i = 0; i < avail; ++i) {
//						cout << setfill('0') << setw(2) << hex << static_cast<int>(rcv[i]) << dec << " ";
						hs << setfill('0') << setw(2) << hex << static_cast<int>(rcv[i]) << dec << " ";
					}
					for (unsigned int i = 0; i < avail; ++i) {
//						cout << setfill(' ') << setw(2) << rcv[i] << " ";
						chs << setw(2) << rcv[i] << " ";
					}
					if (avail == 1) {
						cout << "(" << avail << ") hex: " << hs.str() << "\t" << "ascii: " << chs.str() << endl;
					} else if (avail > 1) {
						cout << "(" << avail << ") hex: " << hs.str() << endl;
						cout << "  ascii: " << chs.str() << endl;
					}
					break;
				}

				default: {
					cout << rcv;
					break;
				}
			}
		} else {
			this_thread::yield();
		}
	}
}

int main(int argCnt, char *argVal[]) {
	for (int i = 0; i < argCnt; ++i) {
		string a = argVal[i];
		if (a.compare("-d") == 0) {
			SERIAL_PORT = argVal[i+1];

		} else if (a.compare("-b") == 0) {
			SERIAL_BAUD = argVal[i+1];

		} else if (a.compare("-a") == 0) {
			txMode = modeASCII;
			rxMode = modeASCII;

		} else if (a.compare("-h") == 0) {
			txMode = modeHEX;
			rxMode = modeHEX;

		}

	}
	cout << "Hey" << endl; // prints Hey

	// Serial object
	SeriaLib serial;

	// Connection to serial port
	unsigned int ui = stoi(SERIAL_BAUD);
	char errorOpening = serial.openDevice(SERIAL_PORT, ui);

	// If connection fails, return the error code otherwise, display a success message
	if (errorOpening != 1) {
		cout << "Error connecting to " << SERIAL_PORT << endl;
		return errorOpening;
	} else {
		cout << "Connected to " << SERIAL_PORT << " at " << SERIAL_BAUD << endl;
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
	thread rrx(rx, serial, rxMode);

	ttx.join();
	serial.closeDevice();    // Close the serial device

	return 0;
}
