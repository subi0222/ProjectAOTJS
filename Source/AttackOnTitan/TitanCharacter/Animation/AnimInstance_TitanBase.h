// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_TitanBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAnimInstance_TitanBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAnimInstance_TitanBase();

	void PlayAttackMontage();

	void PlayDieMontage();

	void JumpToAttackMontageSection(int32 NewSection);

	void JumpToDieMontageSection(int32 NewSection);

	virtual void PostInitProperties() override;

	//∏Æ∆—≈‰∏µ ¡ﬂ....
	void JumpToComboAttackMontageSection(int32 CurrentCombo, UAnimMontage*& ComboAttackMontage);

	void PlayAttackMontage(UAnimMontage* AttackMontage);

	void ChangeToMiddleAttackMontage();

	void ChangeToUpperAttackMontage();

	void ChangeToLowerAttackMontage();

protected:

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_DieCheck();

	FName GetAttackMontageSectionName(int32 Index);

	FName GetDieMontageSectionName(int32 Index);

	FName GetComboAttackMontageSectionName(int32 CurrentCombo);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> MeleeMiddleAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> MeleeUpperAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> MeleeLowerAttackMontage;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> CurrentMeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> DieMontage;

	
};
