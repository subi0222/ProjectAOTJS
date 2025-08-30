// Unreal Engine Marketplace Copyright


#include "AnimInstance_TitanBase.h"
#include "Animation/AnimMontage.h"

UAnimInstance_TitanBase::UAnimInstance_TitanBase()
{
}

void UAnimInstance_TitanBase::PlayAttackMontage()
{
	Montage_Play(CurrentMeleeAttackMontage);
}

void UAnimInstance_TitanBase::PlayDieMontage()
{
	Montage_Play(DieMontage);
}

void UAnimInstance_TitanBase::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetComboAttackMontageSectionName(NewSection), CurrentMeleeAttackMontage);
	FString Result = CurrentMeleeAttackMontage->IsValidSectionName(GetComboAttackMontageSectionName(NewSection))? "Yes" : "No";
	FString SectionName = GetComboAttackMontageSectionName(NewSection).ToString();
	UE_LOG(LogTemp, Display, TEXT("Section Name : %s"), *SectionName);
	UE_LOG(LogTemp, Display, TEXT("Can Jump To Section : %s"), *Result);
}

void UAnimInstance_TitanBase::JumpToDieMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetDieMontageSectionName(NewSection), DieMontage);
}

void UAnimInstance_TitanBase::JumpToComboAttackMontageSection(int32 CurrentCombo, UAnimMontage*& ComboAttackMontage)
{
	Montage_JumpToSection(GetComboAttackMontageSectionName(CurrentCombo), ComboAttackMontage);
}

void UAnimInstance_TitanBase::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	Montage_Play(AttackMontage);
}

void UAnimInstance_TitanBase::ChangeToMiddleAttackMontage()
{
	if (CurrentMeleeAttackMontage == MeleeMiddleAttackMontage)
		return;
	CurrentMeleeAttackMontage = MeleeMiddleAttackMontage;
}

void UAnimInstance_TitanBase::ChangeToUpperAttackMontage()
{
	if (CurrentMeleeAttackMontage == MeleeUpperAttackMontage)
		return;
	CurrentMeleeAttackMontage = MeleeUpperAttackMontage;
}

void UAnimInstance_TitanBase::ChangeToLowerAttackMontage()
{
	if (CurrentMeleeAttackMontage == MeleeLowerAttackMontage)
		return;
	CurrentMeleeAttackMontage = MeleeLowerAttackMontage;
}

void UAnimInstance_TitanBase::PostInitProperties()
{
	Super::PostInitProperties();
}

void UAnimInstance_TitanBase::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UAnimInstance_TitanBase::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UAnimInstance_TitanBase::AnimNotify_DieCheck() 
{
	TryGetPawnOwner()->Destroy();
}

FName UAnimInstance_TitanBase::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

FName UAnimInstance_TitanBase::GetDieMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Die%d"), Section));
}


// 리팩토링 중....
FName UAnimInstance_TitanBase::GetComboAttackMontageSectionName(int32 CurrentCombo)
{
	return FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo));
}