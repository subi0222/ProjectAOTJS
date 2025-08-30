// Unreal Engine Marketplace Copyright


#include "UI/AOTUW_PlayerStat.h"
#include "Components/TextBlock.h"

void UAOTUW_PlayerStat::SetPlayerStat(StatType Type, float NewValue)
{
	switch (Type)
	{
	case StatType::CurrentDamage: CurrentDamage->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewValue))); break;
	case StatType::AttackRadius: AttackRadius->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewValue))); break;
	case StatType::AttackRange: AttackRange->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewValue))); break;
	}
}

void UAOTUW_PlayerStat::NativeConstruct()
{
	Super::NativeConstruct();
}
