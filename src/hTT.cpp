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
						sr.flushReceiver();
						for (unsigned int i = 0; i < hexCharovi.size(); ++i) {
							sr.writeChar(hexCharovi.at(i));
							usleep(250);
						}
						cout << endl;
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
				this_thread::yield();	// svakako yield

				if (isRedirekcija) {	// ako je redirekcija ulaza, potrebna je pauza izmedju poslatih linija
					// 500 * 10ms = 5 sekundi
					for (int i = 0; i < 500; ++i) {
						this_thread::sleep_for(10ms);
						this_thread::yield();
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
			switch (rxMod) {
				case modeASCII:{
					sr.readString(rcv, '\r', rcvSize - 1, 0);
					/*
					 * FULL FREEZE ASCII MODE - dobijam jednu po jednu _kompletnu_ liniju, kako stizu iz remote uredjaja
					 * ----------------------
					 * delimiter = \r ili 0xd. Portable: \r je system independent, a 0xd je linux specific.
					 * timeout = 0 (izgleda da to znaci beskonacno)
					 * Ako je timeout > 0, pa istekne dok jos pristizu karakteri, getline vraca nekompletnu liniju tj ono sto je dotle stiglo.
					 * Za ascii mode od modema to nije dobro.
					 */
					string ociscen = string(rcv);
					ociscen.erase(std::remove(ociscen.begin(), ociscen.end(), '\r'), ociscen.end());
					ociscen.erase(std::remove(ociscen.begin(), ociscen.end(), '\n'), ociscen.end());
					cout << ociscen << endl;
					break;
				}

				case modeHEX:{
					sr.readBytes(rcv, rcvSize, 25, 200);
					stringstream  hs;
					stringstream  chs;
					for (unsigned int i = 0; i < avail; ++i) {
						hs << setfill('0') << setw(2) << hex << static_cast<int>(rcv[i]) << dec << " ";
					}
					for (unsigned int i = 0; i < avail; ++i) {
						char c = rcv[i];
						if( (c >= 0x20) && (c<=0x7e) ){
							chs << setfill(' ') << setw(2) << rcv[i] << " ";
						} else {
							chs << setfill(' ') << setw(2) << " " << " ";
						}
					}
					if (avail == 1) {
						/*
						 * ako je samo jedan karakter, prikaz je vertikalan
						 * (1) hex: 	7a 	ascii:  z
						 * (1) hex: 	6f 	ascii:  o
						 * (1) hex: 	74 	ascii:  t
						 * (1) hex: 	6a 	ascii:  j
						 * (1) hex: 	61 	ascii:  a
						 */
						cout << "(" << avail << ") hex: \t" << hs.str() << "\t" << "ascii: " << chs.str() << endl;
					} else if (avail > 1) {
						/*
						 * ako je vise karaktera, idemo horizontalno
						 * (8) chars
						 *   hex: 6f 64 67 6f 76 6f 72 20
						 * ascii:  o  d  g  o  v  o  r
						 */
						cout << endl;
						cout << "rx (" << avail << ") chars" << endl;
						cout << "  hex: " << hs.str() << endl;
						cout << "ascii: " << chs.str() << endl;
						cout << endl;
					}
					break;
				}

				default: {
					cout << "Unknown operation mode! Neigher ascii or hex. Check your command line." << endl;
					cout << "received: " << rcv;
					break;
				}
			}
		} else {
			this_thread::sleep_for(50ms);
			this_thread::yield();
		}
	}
}

int main(int argCnt, char *argVal[]) {
	for (int i = 0; i < argCnt; ++i) {
		string a = argVal[i];
		if ( (a.compare("-d") == 0) || ((a.compare("-D") == 0)) ) {
			SERIAL_PORT = argVal[i+1];

		} else if (a.compare("-b") == 0) {
			SERIAL_BAUD = argVal[i+1];

		} else if (a.compare("-a") == 0) {
			txMode = modeASCII;
			rxMode = modeASCII;

		} else if (a.compare("-h") == 0) {
			txMode = modeHEX;
			rxMode = modeHEX;

		} else if ( ((a.compare("--help") == 0)) || ((a.compare("-?") == 0)) ) {
			string usage = "usage: hTT -d <device> -b <baud> [-a|-h]	\n";
			usage += "-d device eg. /dev/ttyUSB0						\n";
			usage += "-b baud_rate for serial port						\n";
			usage += "-[a|h] ascii or hex mode							\n";
			usage += "-? --help this help message						\n";
			usage += "													\n";
			usage += "example:											\n";
			usage += "    hTT -d /dev/ttyUSB4 -b 9600 -a	(test AT commands of a modem)					\n";
			usage += "    hTT -d /dev/ttyS0   -b 9600 -h	(test hex commands of a proprietary device)		\n";

			cout << "Welcome to " << string(argVal[0]) << ", a hex Terminal Tester" << endl;
			cout << endl;
			cout << usage << endl;
			return 0;

			usage += "ex: hTT -d /dev/ttyUSB0 -b 9600 -h				\n";

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
