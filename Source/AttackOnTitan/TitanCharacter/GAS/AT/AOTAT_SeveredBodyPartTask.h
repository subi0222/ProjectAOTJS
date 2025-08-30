// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AOTAT_SeveredBodyPartTask.generated.h"

class UAOTTitanSeveredPartComponent;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTAT_SeveredBodyPartTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	static UAOTAT_SeveredBodyPartTask* CreateSeveredBodyPartTask (UGameplayAbility* OwningAbility,
	const UAOTTitanSeveredPartComponent* SeveredPartComponent);

	virtual void Activate() override;

	UFUNCTION()
	void StartTask();

private:
	
	UPROPERTY()
	const UAOTTitanSeveredPartComponent* SeveredPartComp;
	
};
