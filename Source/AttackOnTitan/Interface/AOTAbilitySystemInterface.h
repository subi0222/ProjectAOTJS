// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AOTAbilitySystemInterface.generated.h"

class UAOTAbilitySystemComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAOTAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ATTACKONTITAN_API IAOTAbilitySystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UAOTAbilitySystemComponent* GetAbilitySystemComponent() = 0;
};
