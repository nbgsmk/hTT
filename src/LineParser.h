/*
 * LineParser.h
 *
 *  Created on: Jul 1, 2024
 *      Author: peca
 */

#ifndef LINEPARSER_H_
#define LINEPARSER_H_

using namespace std;

class LineParser {
	public:
		LineParser();
		virtual ~LineParser();

		void setLogState(bool loggingEnable);
		bool getLogState();

		void setLogLevel(int logLevel);
		int getLogLevel();

		vector<uint8_t> parse(string linija);
		void logPrint(vector<string> stringovi, vector<int> intovi, vector<uint8_t> charovi);

	private:
		bool logEnabled;
		int logLevel;
};

#endif /* LINEPARSER_H_ */
