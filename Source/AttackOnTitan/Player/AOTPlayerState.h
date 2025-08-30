// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AOTPlayerState.generated.h"

class UAOTPlayerAbilitySystemComponent;
class UAOTPlayerAttributeSet;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTPlayerState : public APlayerState , public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAOTPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(EditAnywhere , Category = GAS, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAOTPlayerAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UAOTPlayerAttributeSet> AttributeSet;

	
};
