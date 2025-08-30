// Unreal Engine Marketplace Copyright


#include "UI/AOTUW_PlayerBar.h"
#include "UI/AOTUW_ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "UI/AOTUW_PlayerStat.h"
#include "UI/AOTUI_SkillSlot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/TextBlock.h"
#include "GAS/AttributeSet/AOTPlayerAttributeSet.h"
#include "GameFramework/Character.h"

void UAOTUW_PlayerBar::NativeConstruct()
{
	Super::NativeConstruct();
	HealthBar->ProgressBar_Main->SetFillColorAndOpacity(FLinearColor::Green);
	HealthBar->ProgressBar_Delay->SetFillColorAndOpacity(FLinearColor::Red);
	GasBar->ProgressBar_Main->SetFillColorAndOpacity(FLinearColor::Yellow);
	GasBar->ProgressBar_Delay->SetFillColorAndOpacity(FLinearColor::White);
	OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
	UAOTPlayerAttributeSet* Attribute = const_cast<UAOTPlayerAttributeSet*>(OwnerASC->GetSet<UAOTPlayerAttributeSet>());
	HealthBar->SetMaxValue(Attribute->GetMaxHealth());
	HealthBar->SetValue(Attribute->GetHealth());
	GasBar->SetMaxValue(Attribute->GetMaxGas());
	GasBar->SetValue(Attribute->GetGas());
	FString AttackRadiusStr = FString::Printf(TEXT("%.1f"), Attribute->GetAttackRadius());
	FString AttackRangeStr = FString::Printf(TEXT("%.1f"),Attribute->GetAttackRange());
	FString AttackPowerStr = FString::Printf(TEXT("%.1f"),Attribute->GetAttackPower());
	
	PlayerStat->AttackRadius->SetText(FText::FromString(AttackRadiusStr));
	PlayerStat->AttackRange->SetText((FText::FromString(AttackRangeStr)));
	PlayerStat->CurrentDamage->SetText(FText::FromString(AttackPowerStr));
	
	Attribute->OnChangeHealth.AddDynamic(this, &UAOTUW_PlayerBar::SetHealthBar);
	Attribute->OnChangeGas.AddDynamic(this, &UAOTUW_PlayerBar::SetGasBar);
	
}


void UAOTUW_PlayerBar::SetHealthBar(float Current, float Max)
{
	HealthBar->SetMaxValue(Max);
	HealthBar->SetValue(Current);
}

void UAOTUW_PlayerBar::SetGasBar(float Current, float Max)
{
	GasBar->SetMaxValue(Max);
	GasBar->SetValue(Current);
}

void UAOTUW_PlayerBar::SetSkillCoolTime(int SkillNum, float CoolTime)
{
	switch (SkillNum)
	{
	case 1:
		Skill01->StartCooldown(CoolTime); break;
	case 2:
		Skill02->StartCooldown(CoolTime); break;
	case 3:
		Skill03->StartCooldown(CoolTime); break;
	case 4:
		Skill04->StartCooldown(CoolTime); break;
	}
}

