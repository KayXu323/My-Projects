#ifndef CREATE_INSTRUCTION_H
#define CREATE_INSTRUCTION_H

#include "global.h"

#define MAX_CHIP_PORTION	4
#define MAX_FISH_NUM 	    4
#define TIME_CHIP_COOK 	    120
#define TIME_COD_COOK 	    80
#define TIME_HADDOCK_COOK 	90
#define TIME_ENTER_TO_SERVE_LIMIT  600
#define TIME_COOKED_TO_SERVE_LIMIT  120

class CreateInstructions
{
public:
	bool CreateInstructionsForOneOrder(OneOrderInfo* pOrderInfo);
	vector<OneCookTimeInfo>* GetInstructions() { return &_vectorOutputInfo; }
	size_t GetOrderServeTimestamp() { return _orderServeTimestamp_Last; }		
	
private:
	vector<OneCookTimeInfo> _vectorOutputInfo;
	size_t _orderServeTimestamp_Last;
	OneOrderInfo* _pOrderInfo;
	
private:
	stack<pair<FoodType, size_t>> _stackFishFryer[MAX_FISH_NUM]; //size_t: cooking time span of one kind of fish
	stack<size_t> _stackChipFryer; //size_t: cooking portions of one time

	size_t ChipCookTime();
	void ChipCookSequence(const int iFullCookNum, const int iRemainder);

	size_t FishCookTime();
	void CreateFishArray(vector<pair<FoodType, size_t>>& vFishTime);
	void FishCookSequence(vector<pair<FoodType, size_t>>& vFishTime, size_t* timeTotal);

	bool CreatChipInstruction(const int serveTime);
	bool CreatFishInstruction(const int serveTime);
	void MergeSameItems();

	void clearStacks();

public:
	CreateInstructions() : _pOrderInfo(NULL), _orderServeTimestamp_Last(0) {}
	~CreateInstructions() {}
};

#endif