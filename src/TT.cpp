//============================================================================
// Name        : TT.cpp
// Author      : z
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "serialib.h"
#include <unistd.h>
#include <stdio.h>

#define SERIAL_PORT "/dev/ttyS0"

using namespace std;


int main() {
	cout << "Hey" << endl; // prints Hey

    // Serial object
    serialib serial;


    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 115200);


    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening!=1) return errorOpening;
    printf ("Successful connection to %s\n",SERIAL_PORT);


    // Display ASCII characters (from 32 to 128)
    for (int c=32;c<128;c++)
    {
        serial.writeChar(c);
        usleep(10000);
    }

    // Close the serial device
    serial.closeDevice();

	return 0;
}
