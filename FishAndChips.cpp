// FishAndChips.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FishAndChips.h"

int main()
{
	FinlayFishAndChips cFinlayFishAndChips;
	return 0;
}
//-------------------------------------------------------
FinlayFishAndChips::FinlayFishAndChips()
	:_pBaseOutput(NULL)
{
	switch (FILE_OR_SCREEN)
	{
	case 1:   //Input from file
		_pBaseOutput = new OutputInstructionsToFile();
		Process_File();
		break;
	case 2:  //Input from keyboard
		_pBaseOutput = new OutputInstructionsToScreen();
		Process_Screen();
		break;
	default:
		return;
	}
}
//-------------------------------------------------------
FinlayFishAndChips::~FinlayFishAndChips()
{
	if (_pBaseOutput != NULL)
	{
		delete _pBaseOutput;
		_pBaseOutput = NULL;
	}
}
//-------------------------------------------------------
bool FinlayFishAndChips::Process_Screen()
{
	string line = "";

	while (getline(cin, line))
	{
		ProcessOneOrder(line);
	}
	return true;
}
//-------------------------------------------------------
bool FinlayFishAndChips::Process_File()
{
	string file = "./Input.txt";
	ifstream sfile((char*)file.c_str());
	if (sfile.is_open() == false)
	{
		printf("Unable to open Input.txt\n");
		return false;
	}

	fstream fOutput;
	fOutput.open("./Output.txt", fstream::in | fstream::out | fstream::trunc);
	OutputInstructionsToFile* pOutputInstructionsToFile = dynamic_cast <OutputInstructionsToFile*> (_pBaseOutput);
	pOutputInstructionsToFile->SetFilePt(&fOutput);

	string line = "";
	while (getline(sfile, line))
	{
		ProcessOneOrder(line);
	}

	fOutput.close();
	return true;
}
//-------------------------------------------------------
bool FinlayFishAndChips::ProcessOneOrder(const string & line)
{
	//read one order, and parse this order to OrderInfo structure
	if (!_cParseOrder.parseLine(line))
	{
		cerr << "Error while parsing line \"" << line << "\"" << endl;
		_pBaseOutput->WriteRejectedOrder(_cParseOrder.GetOrderInfoP());
		return false;
	}
	//creat cooking instructions and order status of current order
	if (!_cCreateInstructions.CreateInstructionsForOneOrder(_cParseOrder.GetOrderInfoP()))
	{
		_pBaseOutput->WriteRejectedOrder(_cParseOrder.GetOrderInfoP());
		return false;
	}
	//output cooking instructions and order status 
	_pBaseOutput->WriteAcceptedOrder(_cParseOrder.GetOrderInfoP(), _cCreateInstructions.GetInstructions(), _cCreateInstructions.GetOrderServeTimestamp());
	return true;
}
//-------------------------------------------------------







