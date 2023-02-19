#ifndef FINLAY_FISH_AND_CHIPS_H
#define FINLAY_FISH_AND_CHIPS_H

#include "ParseOrder.h"
#include "CreateInstructions.h"
#include "OutputInstructions.h"

#define FILE_OR_SCREEN  	1   //1: file, 2:screen 

class FinlayFishAndChips
{
private:
	bool Process_File();
	bool Process_Screen();

	bool ProcessOneOrder(const string & line);

	ParseOrder _cParseOrder;
	CreateInstructions _cCreateInstructions;
	BaseOutput* _pBaseOutput;

public:

	FinlayFishAndChips();
	~FinlayFishAndChips();
};

#endif