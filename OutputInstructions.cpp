
#include "stdafx.h"
#include "OutputInstructions.h"

//-------------------------------------------------------
void OutputInstructionsToFile::WriteRejectedOrder(OneOrderInfo* pOrderInfo)
{
	string strTime = timestampToString(pOrderInfo->orderEntryHour, pOrderInfo->orderEntryMinute, pOrderInfo->orderEntrySecond);
	*_fOutput << "at " << strTime.c_str() << ", Order #" << pOrderInfo->Id << " Rejected" << endl;
}
//-------------------------------------------------------
void OutputInstructionsToFile::WriteAcceptedOrder(OneOrderInfo* pOrderInfo, vector<OneCookTimeInfo>* vectorOutputInfo, const size_t serveTime)
{
	string strTime = "", strFoodType = "";
	vector<OneCookTimeInfo> ::iterator it;

	strTime = timestampToString(pOrderInfo->orderEntryHour, pOrderInfo->orderEntryMinute, pOrderInfo->orderEntrySecond);
	*_fOutput << "at " << strTime.c_str() << ", Order #" << pOrderInfo->Id << " Accepted" << endl;

	for (it = vectorOutputInfo->begin(); it != vectorOutputInfo->end(); ++it)
	{
		strTime = timestampToString(it->beginCookTime);
		strFoodType = getNameString(it->foodType);
		*_fOutput << "at " << strTime.c_str() << ", Begin Cooking " << it->foodNum << " " << strFoodType.c_str() << endl;
	}

	strTime = timestampToString(serveTime);
	*_fOutput << "at " << strTime.c_str() << ", Serve Order #" << pOrderInfo->Id << endl;
}
//-------------------------------------------------------
void OutputInstructionsToScreen::WriteRejectedOrder(OneOrderInfo* pOrderInfo)
{
	string strTime = timestampToString(pOrderInfo->orderEntryHour, pOrderInfo->orderEntryMinute, pOrderInfo->orderEntrySecond);
	printf("at %s, Order #%d Rejected\n", strTime.c_str(), pOrderInfo->Id);
}
//-------------------------------------------------------
void OutputInstructionsToScreen::WriteAcceptedOrder(OneOrderInfo* pOrderInfo, vector<OneCookTimeInfo>* vectorOutputInfo, const size_t serveTime)
{
	string strTime = "", strFoodType = "";
	vector<OneCookTimeInfo> ::iterator it;

	strTime = timestampToString(pOrderInfo->orderEntryHour, pOrderInfo->orderEntryMinute, pOrderInfo->orderEntrySecond);
	printf("at %s, Order #%d Accepted\n", strTime.c_str(), pOrderInfo->Id);

	for (it = vectorOutputInfo->begin(); it != vectorOutputInfo->end(); ++it)
	{
		strTime = timestampToString(it->beginCookTime);
		strFoodType = getNameString(it->foodType);
		printf("at %s, Begin Cooking %d %s\n", strTime.c_str(), it->foodNum, strFoodType.c_str());
	}

	strTime = timestampToString(serveTime);
	printf("at %s, Serve Order #%d\n", strTime.c_str(), pOrderInfo->Id);
}
//-------------------------------------------------------
string BaseOutput::timestampToString(int secondsSinceMidnight)
{
	const int hour = secondsSinceMidnight / 3600;
	const int minute = (secondsSinceMidnight / 60) % 60;
	const int second = secondsSinceMidnight % 60;

	ostringstream timestamp;

	timestamp << setfill('0') << setw(2) << hour << ":" << setw(2) << minute << ":" << setw(2) << second;

	return timestamp.str();
}
//-------------------------------------------------------
string BaseOutput::timestampToString(int hour, int minute, int second)
{
	ostringstream timestamp;

	timestamp << setfill('0') << setw(2) << hour << ":" << setw(2) << minute << ":" << setw(2) << second;

	return timestamp.str();
}
//-------------------------------------------------------
string BaseOutput::getNameString(FoodType food)
{
	switch (food)
	{
	case BatteredCod:
		return "Cod";
	case BatteredHaddock:
		return "Haddock";
	case Chips:
		return "Chips";
	default:
		return "";
	}
}
//-------------------------------------------------------