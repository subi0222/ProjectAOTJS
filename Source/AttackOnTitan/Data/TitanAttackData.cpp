// Unreal Engine Marketplace Copyright


#include "Data/TitanAttackData.h"

TArray<FAttackData> UTitanAttackData::GetAttackDataArray() const
{
	return AttackDataArray;
}

int8 UTitanAttackData::GetSequenceNum() const
{
	return SequenceNum;
}

FAttackData UTitanAttackData::GetAttackData(int32 Index) const
{
	return AttackDataArray[Index];
}
