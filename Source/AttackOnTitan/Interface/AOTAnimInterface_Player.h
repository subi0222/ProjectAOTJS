// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AOTAnimInterface_Player.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAOTAnimInterface_Player : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ATTACKONTITAN_API IAOTAnimInterface_Player
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RotateToTarget() = 0;
	virtual void ChangeMode() = 0;
};
