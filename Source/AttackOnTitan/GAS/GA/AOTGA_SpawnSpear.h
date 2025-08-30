// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AOTGA_SpawnSpear.generated.h"

class ACharacter;
class AAOTWeapon_Spear;
class AActor;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTGA_SpawnSpear : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAOTGA_SpawnSpear();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


protected:
	AActor* DetectEnemy(float Range, float Radius, bool Single);
	
private:
	UPROPERTY(EditAnywhere, Category = Spear)
	TSubclassOf<AAOTWeapon_Spear> SpearClass;
	UPROPERTY()
	TObjectPtr<AAOTWeapon_Spear> SpawnedSpear;
	FTimerHandle SpanTimeTimer;
};
