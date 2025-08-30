// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TitanCharacter/Data/AOTTitanAttackData.h"
#include "AOTAT_TitanSpawnActorTask.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTAT_TitanSpawnActorTask : public UAbilityTask
{
	GENERATED_BODY()

public:

	static UAOTAT_TitanSpawnActorTask* CreateSpawnActorTask(UGameplayAbility* OwningAbility,
		TArray<FSpawnActorInformation> SpawnActorInformations,
		ACharacter* Target);

	virtual void Activate() override;

protected:

	void SpawnActor(FSpawnActorInformation SpawnActorInformation);

protected:

	UPROPERTY()
	TArray<FSpawnActorInformation> SpawnActorInformations;

	UPROPERTY()
	ACharacter* Target;

	UPROPERTY()
	AActor* Weapon;
	
};
