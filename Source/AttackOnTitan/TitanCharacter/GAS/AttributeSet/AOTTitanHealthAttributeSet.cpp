// Unreal Engine Marketplace Copyright

#include "TitanCharacter/GAS/AttributeSet/AOTTitanHealthAttributeSet.h"

#include "AttackOnTitan.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/Components/AOTTitanHealthComponent.h"

void UAOTTitanHealthAttributeSet::ApplyDamage(float DamageAmount)
{
	AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(GetOwningActor());
	UAOTTitanHealthComponent* TitanHealthComp = TitanBase->GetComponentByClass<UAOTTitanHealthComponent>();
	if (!TitanHealthComp)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), DamageAmount);
	if (HP.GetCurrentValue() <= 0)
	{
		//이미 HP가 0인 상태이므로 데미지를 받을 필요 없음
		AOT_LOG(LogAOT, Log, TEXT("%s"), TEXT("Already Hp Is Zero"));
		return;
	}
	float NewHP = FMath::Clamp(HP.GetCurrentValue() - DamageAmount, 0.0f, HP.GetBaseValue());
	HP.SetCurrentValue(NewHP);
	float RemainHPRate = HP.GetCurrentValue() / HP.GetBaseValue();
	UE_LOG(LogTemp, Warning, TEXT("RemainHP : %f"), RemainHPRate);
	TitanHealthComp->OnTitanHealthChangedDelegate.Broadcast(HP.GetCurrentValue()/HP.GetBaseValue());
	if (RemainHPRate <= 0)
	{
		TitanHealthComp->TriggerDieEvent();
		TitanHealthComp->OnTitanDieStartDelegate.Broadcast();
	}
	InitDamage(0);
}

void UAOTTitanHealthAttributeSet::ApplyStaminaCost(float StaminaCostAmount)
{
	AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(GetOwningActor());
	AOT_LOG(LogAOTGAS, Log, TEXT("Now CurrentStamina : %f"), Stamina.GetCurrentValue());
	float NewStamina = FMath::Clamp(Stamina.GetCurrentValue() - StaminaCostAmount, 0.0f, Stamina.GetCurrentValue());
	AOT_LOG(LogAOTGAS, Log, TEXT("Stamina : %f"), NewStamina);
	Stamina.SetCurrentValue(NewStamina);
	InitStaminaCost(0.f);
}

void UAOTTitanHealthAttributeSet::ApplyStamina(float StaminaAmount)
{
	float NewStamina = FMath::Clamp(Stamina.GetCurrentValue() + StaminaAmount, 0.0f, Stamina.GetBaseValue());
	Stamina.SetCurrentValue(NewStamina);
}

UAOTTitanAbilitySystemComponent* UAOTTitanHealthAttributeSet::GetTitanAbilitySystemComponent()
{
	return Cast<UAOTTitanAbilitySystemComponent>(GetOwningAbilitySystemComponent()); 
}

void UAOTTitanHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	if (Attribute == GetDamageAttribute())
	{
		ApplyDamage(NewValue);
	}

	if (Attribute == GetStaminaCostAttribute())
	{
		ApplyStaminaCost(NewValue);
	}

	if (Attribute == GetStaminaAttribute())
	{
		ApplyStamina(NewValue);
	}
		
}
