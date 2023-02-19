#ifndef GLOBAL_H  
#define GLOBAL_H

using namespace std;

#include <memory.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stack> 
#include <fstream>
#include <algorithm> 

#pragma pack(1)
typedef enum
{
	BatteredCod = 1,
	BatteredHaddock,
	Chips
}FoodType;

typedef struct
{
	int Id;
	int orderEntryHour;
	int orderEntryMinute;
	int orderEntrySecond;
	size_t CodNum;
	size_t HaddockNum;
	size_t ChipNum;
}OneOrderInfo;

typedef struct
{
	size_t beginCookTime;
	FoodType foodType;
	size_t foodNum;
}OneCookTimeInfo;

#pragma pack()

#endif

