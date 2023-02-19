#ifndef PARSE_ORDER_H  
#define PARSE_ORDER_H

#include "global.h"

class ParseOrder
{
public: 
	bool parseLine(const string & line);
	OneOrderInfo* GetOrderInfoP() { return &_stOrderInfo; }

private:
	OneOrderInfo _stOrderInfo;

	void parseLineEnding(const string & line, size_t numberOfCharactersScanned, vector<string> & parts);
	bool parseLinePart(const string & linePart);
};

#endif