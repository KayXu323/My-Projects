
#include "stdafx.h"
#include "ParseOrder.h"

/*********************************************************************************
Function:      parseLine
Description£º  Read one order, and parse this order
Input:         line: one order string
Output:		   _stOrderInfo:  parsed information from order string
Return:		   true:  order meets format
               false: order doesn't meet format
**********************************************************************************/
bool ParseOrder::parseLine(const string & line)
{
	size_t numberOfCharactersScanned = 0;
	memset(&_stOrderInfo, 0, sizeof(OneOrderInfo));

	// parse the order id and timestamp
	const int parametersRead = sscanf_s(line.c_str(),
		"Order #%d, %2d:%2d:%2d, %zn",   // %zn: position
		&_stOrderInfo.Id,
		&_stOrderInfo.orderEntryHour,
		&_stOrderInfo.orderEntryMinute,
		&_stOrderInfo.orderEntrySecond,
		&numberOfCharactersScanned);
	if (parametersRead != 4)
	{
		return false; // not enough parameters
	}

	// parse all the order items
	vector<string> parts;
	parseLineEnding(line, numberOfCharactersScanned, parts);
	for (const auto & part : parts)
	{
		if (!parseLinePart(part))
		{
			return false; // bad order format
		}
	}

	return true;
}

//-------------------------------------------------------
void ParseOrder::parseLineEnding(const string & line, size_t numberOfCharactersScanned, vector<string> & parts)
{
	size_t nextPartBegins = line.find(',', numberOfCharactersScanned);
	while (nextPartBegins != string::npos)
	{
		parts.push_back(line.substr(numberOfCharactersScanned, nextPartBegins - numberOfCharactersScanned));

		numberOfCharactersScanned = nextPartBegins + 1;
		nextPartBegins = line.find(',', numberOfCharactersScanned);
	}

	parts.push_back(line.substr(numberOfCharactersScanned));
}
//-------------------------------------------------------
bool ParseOrder::parseLinePart(const string & linePart)
{
	size_t count = 0;
	size_t numberOfCharactersScanned = 0;
	string name = "";

	int parametersRead = sscanf_s(linePart.c_str(), " %zu %zn", &count, &numberOfCharactersScanned);
	if (parametersRead != 1)
	{
		return false; //  not enough parameters
	}

	name = linePart.substr(numberOfCharactersScanned);

	if (name == "Cod")
		_stOrderInfo.CodNum = count;
	else if (name == "Haddock")
		_stOrderInfo.HaddockNum = count;
	else if (name == "Chips")
		_stOrderInfo.ChipNum = count;
	else
		return false;

	return true;
}
//-------------------------------------------------------