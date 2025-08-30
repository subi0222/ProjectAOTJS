// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AOTTitanAttackInterFace.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAOTTitanAttackInterFace : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ATTACKONTITAN_API IAOTTitanAttackInterFace
{
	GENERATED_BODY()
	
	virtual void TryAttack(FGameplayTag AttackTag) = 0;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
