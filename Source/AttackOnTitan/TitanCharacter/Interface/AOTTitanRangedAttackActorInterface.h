// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AOTTitanRangedAttackActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAOTTitanRangedAttackActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ATTACKONTITAN_API IAOTTitanRangedAttackActorInterface
{
	GENERATED_BODY()
	
public:

	virtual void FollowTarget(ACharacter* Character) = 0;

	virtual bool HitCheck() = 0;

	virtual void SetDamage(float Damage) = 0;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
