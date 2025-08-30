// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "AOTTitanAttackPartComponent.generated.h"

class ABaseCharacter;
class AAOTTitanBase;
/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATTACKONTITAN_API UAOTTitanAttackPartComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UAOTTitanAttackPartComponent();

	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintCallable)
	void MakeAttackHitCheckEvent();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void EmptyHitCharacters() { HitCharacters.Empty(); }

	FORCEINLINE bool HasHit(ABaseCharacter* HitCharacter) const { return HitCharacters.Contains(HitCharacter); }

	//GameAbility AttackHitCheck에서 추가함
	void RegisterHitCharacter(ABaseCharacter* HitCharacter);

private:
	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase;

	UPROPERTY()
	TArray<ABaseCharacter*> HitCharacters;
};
