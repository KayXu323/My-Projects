#ifndef OUTPUT_INSTRUCTION_H 
#define OUTPUT_INSTRUCTION_H

#include "global.h"

class BaseOutput
{
public:
	virtual void WriteAcceptedOrder(OneOrderInfo* pOrderInfo, vector<OneCookTimeInfo>* vectorOutputInfo, const size_t serveTime) = 0;
	virtual void WriteRejectedOrder(OneOrderInfo* pOrderInfo) = 0;

protected:
	string timestampToString(int secondsSinceMidnight);
	string timestampToString(int hour, int minute, int second);
	string getNameString(FoodType food);
};


class OutputInstructionsToFile : public BaseOutput
{
public:
	virtual void WriteAcceptedOrder(OneOrderInfo* pOrderInfo, vector<OneCookTimeInfo>* vectorOutputInfo, const size_t serveTime);
	virtual void WriteRejectedOrder(OneOrderInfo* pOrderInfo);

	void SetFilePt(fstream* fOutput) { _fOutput = fOutput; }

private:
	fstream* _fOutput;
};


class OutputInstructionsToScreen : public BaseOutput
{
public:
	virtual void WriteAcceptedOrder(OneOrderInfo* pOrderInfo, vector<OneCookTimeInfo>* vectorOutputInfo, const size_t serveTime);
	virtual void WriteRejectedOrder(OneOrderInfo* pOrderInfo);
};

#endif