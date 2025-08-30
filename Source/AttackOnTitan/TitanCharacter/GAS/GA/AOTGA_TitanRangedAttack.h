// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "TitanCharacter/GAS/GA/AOTTitanGameplayAbility.h"
#include "AOTGA_TitanRangedAttack.generated.h"

/**
 * 
 */

class AAOTTitanProjectile;
class UAbilityTask_WaitGameplayEvent;

UENUM(BlueprintType)
enum class ERangedAttackType : uint8
{
	RollThrow UMETA(DisplayName = "RollThrow"),
	NormalThrow UMETA(DisplayName = "NormalThrow"),
	ScatterThrow UMETA(DisplayName = "ScatterThrow")
};

UCLASS()
class ATTACKONTITAN_API UAOTGA_TitanRangedAttack : public UAOTTitanGameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

private:

	UFUNCTION()
	void HandleCancelled();

	UFUNCTION() 
	void HandleEnd();

	UFUNCTION()
	void HandleProjectileGrabbed(FGameplayEventData Payload);

	UFUNCTION()
	void HandleProjectileNormalThrown(FGameplayEventData Payload);

	UFUNCTION()
	void HandleProjectileScatterThrown(FGameplayEventData Payload);

	UFUNCTION()
	void HandleProjectileRollThrown(FGameplayEventData Payload);

	UFUNCTION()
	void HandleProjectileSmashed(FGameplayEventData Payload);

	UAbilityTask_WaitGameplayEvent* CreateForThrowEventTask();

	void BindEventTask(UAbilityTask_WaitGameplayEvent* EventTask);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Throw")
	TSubclassOf<AAOTTitanProjectile> ProjectileClass; 

	UPROPERTY(EditDefaultsOnly, Category = "Throw")
	TObjectPtr<UAnimMontage> ThrowMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Throw")
	ERangedAttackType RangedAttackType;

	UPROPERTY(EditDefaultsOnly, Category = "Throw")
	FName AttatchSocketName;

	UPROPERTY(VisibleInstanceOnly, Category = "Throw")
	TArray<AAOTTitanProjectile*> Projectiles;

	UPROPERTY(VisibleInstanceOnly, Category = "Throw")
	ACharacter* TargetCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Throw")
	uint8 ProjectileNum;

	UPROPERTY(EditDefaultsOnly, Category = "Throw")
	float ThrowTime;
	
	
};
