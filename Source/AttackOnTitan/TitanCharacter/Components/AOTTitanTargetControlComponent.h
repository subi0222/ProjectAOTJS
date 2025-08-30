// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AOTTitanTargetControlComponent.generated.h"


class UAOTTitanSeveredPartComponent;
class UAOTTitanStatData;
class AAOTTitanBase;

UENUM(BlueprintType)
enum class ETargetChangeMode : uint8
{
	ChangeTargetEachHit UMETA(DisplayName = "Change Target Each Hit"),
	ChangeTargetWhenSevered UMETA(DisplayName = "Change Target When Severed"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTACKONTITAN_API UAOTTitanTargetControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAOTTitanTargetControlComponent();
	
	virtual void InitializeComponent() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void UpdateBodyTarget(UAOTTitanSeveredPartComponent* SeveredPartComponent);

	UFUNCTION()
	void SeveredBodyTarget(UAOTTitanSeveredPartComponent* SeveredPartComponent);

	void RegisterBodyTargets(TArray<UAOTTitanSeveredPartComponent*> SeveredPartComponents);

protected:

	void ChangeBodyTargetFrom(UAOTTitanSeveredPartComponent* TitanSeveredPartComponent);

private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	ETargetChangeMode TargetChangeMode;
	
	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase; 

	UPROPERTY(VisibleInstanceOnly)
	TArray<TObjectPtr<UAOTTitanSeveredPartComponent>> ControlledSeveredParts;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<UAOTTitanSeveredPartComponent> WeaknessSeveredPart;
};
