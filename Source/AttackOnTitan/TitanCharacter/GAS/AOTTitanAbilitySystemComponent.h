// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AOTTitanAbilitySystemComponent.generated.h"

class AAOTTitanBase;
class UAOTTitanAttackData;
enum class EAttackMode : uint8;
class UAOTTitanGameAbility;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ATTACKONTITAN_API UAOTTitanAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	AAOTTitanBase* GetTitanOwner() const { return TitanOwner; }

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

private:

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanOwner;
	

};