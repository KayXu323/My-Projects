
#include "stdafx.h"
#include "CreateInstructions.h"

//---------------------------------------------
bool compOutput(const OneCookTimeInfo & a, const OneCookTimeInfo & b)
{
	if (a.beginCookTime < b.beginCookTime)
		return true;
	else if (a.beginCookTime > b.beginCookTime)
		return false;
	else
		return a.foodType < b.foodType;
}
/*********************************************************************************
Function:      CreateInstructionsForOneOrder
Description£º  create cooking instrctions of current order
Input:         pOrderInfo:  parsed information from order string
Output:		   _vectorOutputInfo: includes all cooking instructions
Return:		   true: food meets good quality
               false: food doesn't meet good quality
**********************************************************************************/
bool CreateInstructions::CreateInstructionsForOneOrder(OneOrderInfo* pOrderInfo)
{
	int orderEntryTimestamp = 0;
	int orderCookingTimeSpan = 0;
	int orderServeTimestamp = 0;
	_pOrderInfo = pOrderInfo;
	_vectorOutputInfo.clear();
	clearStacks();

	//enter time of current order
	orderEntryTimestamp = _pOrderInfo->orderEntryHour * 3600 + _pOrderInfo->orderEntryMinute * 60 + _pOrderInfo->orderEntrySecond;

	//cooking time of current order
	int iChipFryerCookTime = ChipCookTime();
	int iFishFryerCookTime = FishCookTime();
	orderCookingTimeSpan = iChipFryerCookTime > iFishFryerCookTime ? iChipFryerCookTime : iFishFryerCookTime;

	//serve time of current order
	if (orderEntryTimestamp < _orderServeTimestamp_Last)
		orderServeTimestamp = _orderServeTimestamp_Last + orderCookingTimeSpan;
	else
		orderServeTimestamp = orderEntryTimestamp + orderCookingTimeSpan;

	//Check whether meet good quality
	if (orderServeTimestamp - orderEntryTimestamp > TIME_ENTER_TO_SERVE_LIMIT)
	{
		printf("Warning: Order #%d will be completed more than %d seconds!\n", _pOrderInfo->Id, TIME_ENTER_TO_SERVE_LIMIT);  
		return false;
	}

	//calculate beginning cooking time for each batch chip, and create chip cooking instruction
	if (false == CreatChipInstruction(orderServeTimestamp))
		return false;

	//calculate beginning cooking time for every fish, and create every fish cooking instruction	
	if (false == CreatFishInstruction(orderServeTimestamp))
		return false;

	//sort cooking instructions according to beginning cooking time and food type 
	sort(_vectorOutputInfo.begin(), _vectorOutputInfo.end(), compOutput);

	//merge same instructions in order to meet output format
	MergeSameItems();

	//save current serve timestamp
	_orderServeTimestamp_Last = orderServeTimestamp;

	return true;
}
/*********************************************************************************
Function:      CreatFishInstruction
Description£º  create each fish cooking instruction
Input:         serveTime: serve time of current order
               _stackFishFryer[MAX_FISH_NUM]: fish cooking sequences
Output:		   _vectorOutputInfo: put each fish cooking instruction(OneCookTimeInfo structure) into a vector
Return:		   true: fish meets good quality
               false: fish doesn't meet good quality
**********************************************************************************/
bool CreateInstructions::CreatFishInstruction(const int serveTime)
{
	bool bAllStackEmpty;
	OneCookTimeInfo stFishOutputInfo[MAX_FISH_NUM];
	size_t endCookTime[MAX_FISH_NUM];
	for (int i = 0; i < MAX_FISH_NUM; i++)
	{
		stFishOutputInfo[i].foodNum = 1;
		endCookTime[i] = serveTime;  
	}

	do
	{
		bAllStackEmpty = true;
		for (int i = 0; i < MAX_FISH_NUM; i++)
		{
			if (!_stackFishFryer[i].empty())
			{
				bAllStackEmpty = false;
				if (serveTime - endCookTime[i] > TIME_COOKED_TO_SERVE_LIMIT)
				{
					printf("Warning: Food in order #%d has been cooked more than %d seconds when served!\n", _pOrderInfo->Id, TIME_COOKED_TO_SERVE_LIMIT);  
					return false;
				}
				stFishOutputInfo[i].foodType = _stackFishFryer[i].top().first;
				stFishOutputInfo[i].beginCookTime = endCookTime[i] - _stackFishFryer[i].top().second;
				_stackFishFryer[i].pop();
				_vectorOutputInfo.push_back(stFishOutputInfo[i]);
				endCookTime[i] = stFishOutputInfo[i].beginCookTime;  //current beginning cooking time == previous ending cooking time
			}
		}
	} while (!bAllStackEmpty);

	return true;
}
/*********************************************************************************
Function:      CreatChipInstruction
Description£º  create chip cooking instruction for each batch
Input:         serveTime: serve time of current order
               _stackChipFryer: chip cooking sequence
Output:		   _vectorOutputInfo: put chip cooking instruction(OneCookTimeInfo structure) for each batch into a vector
Return:		   true: chips meet good quality
               false: chips don't meet good quality
**********************************************************************************/
bool CreateInstructions::CreatChipInstruction(const int serveTime)
{
	OneCookTimeInfo stOutputInfo;
	stOutputInfo.foodType = Chips;
	size_t endCookTime = serveTime;

	while (!_stackChipFryer.empty())
	{		
		if (serveTime - endCookTime > TIME_COOKED_TO_SERVE_LIMIT)
		{
			printf("Warning: Food in order #%d has been cooked more than %d seconds when served!\n", _pOrderInfo->Id, TIME_COOKED_TO_SERVE_LIMIT); 
			return false;
		}
		stOutputInfo.foodNum = _stackChipFryer.top();
		_stackChipFryer.pop();
		stOutputInfo.beginCookTime = endCookTime - TIME_CHIP_COOK;
		_vectorOutputInfo.push_back(stOutputInfo);
		endCookTime = stOutputInfo.beginCookTime;  //current beginning cooking time == previous ending cooking time
	}
	return true;
}
/*********************************************************************************
Function:      ChipCookTime
Description£º  Calculate chip cooking time span of current order
Input:         _pOrderInfo->ChipNum: the number of chips of current order
Output:		   _stackChipFryer: chip cooking sequence
Return:		   chip cooking time span
**********************************************************************************/
size_t CreateInstructions::ChipCookTime()
{
	int iFullCookNum = _pOrderInfo->ChipNum / MAX_CHIP_PORTION;
	int iRemainder = _pOrderInfo->ChipNum % MAX_CHIP_PORTION;

	//create chip cooking sequence, and put them into stack
	ChipCookSequence(iFullCookNum, iRemainder);

	//calculate total chip cooking time of current order
	if (0 != iRemainder)
		return (iFullCookNum + 1) * TIME_CHIP_COOK;
	else
		return (iFullCookNum * TIME_CHIP_COOK);
}
/*********************************************************************************
Function:      ChipCookSequence
Description£º  Create chip cooking sequence, and put them into stack
Input:         iFullCookNum: the number of batches with full portions
               iRemainder: remainder portions;
Output:		   _stackChipFryer: chip cooking sequence
Return:		   N/A
**********************************************************************************/
void CreateInstructions::ChipCookSequence(const int iFullCookNum, const int iRemainder)
{
	if (0 != iRemainder)
		_stackChipFryer.push(iRemainder);
	for (int i = 0; i < iFullCookNum; i++)
		_stackChipFryer.push(MAX_CHIP_PORTION);
}
/*********************************************************************************
Function:      FishCookTime
Description£º  Calculate fish cooking time span of current order
Input:         _pOrderInfo->CodNum: the number of cod
               _pOrderInfo->HaddockNum: the number of haddock
Output:		   _stackFishFryer[MAX_FISH_NUM]: fish cooking sequence for each stack
Return:		   fish cooking time span
**********************************************************************************/
size_t CreateInstructions::FishCookTime()
{
	vector<pair<FoodType, size_t>> vFishTime;

	//Put all fish of current order into a vector
	CreateFishArray(vFishTime);

	//Calculate the quickest cooking time span of all fish, and create fish cooking sequences for each fish stack
	size_t timeTotal[MAX_FISH_NUM];
	memset(timeTotal, 0, sizeof(timeTotal));
	FishCookSequence(vFishTime, timeTotal);

	//calculate all fish cooking time span
	size_t maxTimeFryer = timeTotal[0];
	for (int i = 1; i < MAX_FISH_NUM; i++)
	{
		if (timeTotal[i] > maxTimeFryer)
			maxTimeFryer = timeTotal[i];
	}
	return maxTimeFryer;
}
//-------------------------------------------------------
bool compFunction(const pair<FoodType, size_t>&a, const pair<FoodType, size_t>&b)
{
	return (a.second>b.second);
}
/*********************************************************************************
Function:      FishCookSequence
Description£º  calculate the quickest cooking time span of all fish, \
               and create fish cooking sequences for each fish stack
Input:         vFishTime: vector includes all fish
Output:		   _stackFishFryer[MAX_FISH_NUM]: fish cooking sequence for each stack
               timeTotal[MAX_FISH_NUM]: total cooking time span for each stack
Return:		   N/A
**********************************************************************************/
void CreateInstructions::FishCookSequence(vector<pair<FoodType, size_t>>& vFishTime, size_t* timeTotal)
{
	//sort vector according to fish cooking time
	sort(vFishTime.begin(), vFishTime.end(), compFunction);

	//get fish from vector and put it into a stack which has minimum cooking time from four stacks
	vector<pair<FoodType, size_t>> ::iterator it;
	for (it = vFishTime.begin(); it != vFishTime.end(); ++it)
	{
		size_t minTimesTotal = timeTotal[0];
		size_t minTimeFryer = 0;
		for (int i = 1; i < MAX_FISH_NUM; i++)
		{
			if (timeTotal[i] < minTimesTotal)
			{
				minTimesTotal = timeTotal[i];
				minTimeFryer = i;
			}
		}
		_stackFishFryer[minTimeFryer].push(make_pair(it->first, it->second));
		timeTotal[minTimeFryer] += it->second;
	}
}
/*********************************************************************************
Function:      CreateFishArray
Description£º  put all fish of current order into a vector
Input:         _pOrderInfo->CodNum: the number of cod
               _pOrderInfo->HaddockNum:the number of haddock
Output:		   vFishTime: put fish type and its cooking time into vector
Return:		   N/A
**********************************************************************************/
void CreateInstructions::CreateFishArray(vector<pair<FoodType, size_t>>& vFishTime)
{
	for (int i = 0; i < _pOrderInfo->CodNum; i++)
	{
		vFishTime.push_back(make_pair(BatteredCod, TIME_COD_COOK));
	}
	for (int i = 0; i < _pOrderInfo->HaddockNum; i++)
	{
		vFishTime.push_back(make_pair(BatteredHaddock, TIME_HADDOCK_COOK));
	}
}
/*********************************************************************************
Function:      MergeSameItems
Description£º  merge same instructions in order to meet output format
Input:         _vectorOutputInfo
Output:		   _vectorOutputInfo
Return:		   N/A
**********************************************************************************/
void CreateInstructions::MergeSameItems()
{
	for (int i = 1; i<_vectorOutputInfo.size(); ++i)
	{
		if ((_vectorOutputInfo[i].beginCookTime == _vectorOutputInfo[i - 1].beginCookTime)
			&& (_vectorOutputInfo[i].foodType == _vectorOutputInfo[i - 1].foodType))
		{
			_vectorOutputInfo[i - 1].foodNum++;
			_vectorOutputInfo.erase(_vectorOutputInfo.begin() + i);
			i--;
		}
	}
}
//-------------------------------------------------------
void CreateInstructions::clearStacks()
{
	//clear chip stack
	while (!_stackChipFryer.empty())
	{
		_stackChipFryer.pop();
	}
	//clear fish stacks
	for (int i = 0; i < MAX_FISH_NUM; i++)
	{
		while (!_stackFishFryer[i].empty())
		{
			_stackFishFryer[i].pop();
		}
	}
}
//-------------------------------------------------------
