// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "AOTTitanSeveredPartComponent.generated.h"

class AAOTTitanBody;
class UAOTTitanHealthComponent;
class UAOTTitanTargetControlComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartTargetStart, UAOTTitanSeveredPartComponent*, SeveredPartComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartTargetSevered, UAOTTitanSeveredPartComponent*, SeveredPartComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartTargetChange, UAOTTitanSeveredPartComponent*, SeveredPartComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartTargetGotHit, UAOTTitanSeveredPartComponent*, SeveredPartComp);

class AAOTTitanBase;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ATTACKONTITAN_API UAOTTitanSeveredPartComponent : public USphereComponent
{
	GENERATED_BODY()

public:

	UAOTTitanSeveredPartComponent();

	FORCEINLINE bool IsWeaknessPart() const { return  bIsWeakness; }

	FORCEINLINE FGameplayTag GetStateTagAfterSevered() const { return StateTagAfterSevered; }
	
	FORCEINLINE FGameplayTag GetRemovedAfterSeveredTitanStateTag() const { return RemovedAfterSeveredTitanStateTag; }

	FORCEINLINE float GetCurrentPartHP() const { return CurrentPartHP; }

	FORCEINLINE float GetPartHPBase() const { return PartHPBase; }

	FORCEINLINE TSubclassOf<AAOTTitanBody> GetDropActorClass() const {return DropItemClass;}

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	UAnimMontage* GetRandomSeveredMontage() const;
	
	UAnimMontage* GetRandomStateTransitionMontage() const;

	//GE를 통해 데미지를 받았을 경우 이 부위를 맞았으면 호출! 
	void GotHit(float Damage);

protected:

	
	UFUNCTION()
	void InitSeveredPart(UAOTTitanSeveredPartComponent* TitanSeveredPartComponent);

	void TriggerSeveredEvent();

public:
	
	FOnPartTargetStart OnPartTargetStarted;

	FOnPartTargetSevered OnPartTargetSevered;

	FOnPartTargetGotHit OnPartTargetGotHit;

private:

	UPROPERTY(EditAnywhere, Category = Tag)
	FGameplayTag RemovedAfterSeveredTitanStateTag;
	
	UPROPERTY(EditAnywhere, Category = Tag)
	FGameplayTag StateTagAfterSevered;

	UPROPERTY(EditAnywhere, Category = Montage)
	TArray<TObjectPtr<UAnimMontage>> SeveredPlayMontages;

	UPROPERTY(EditAnywhere, Category = Montage)
	TArray<TObjectPtr<UAnimMontage>> StateTransitionMontages;

	UPROPERTY(EditAnywhere, Category = DropItem)
	TSubclassOf<AAOTTitanBody> DropItemClass;
	
	UPROPERTY(EditAnywhere, Category = Weakness)
	bool bIsWeakness;

	UPROPERTY(EditAnywhere, Category = HP)
	float PartHPBase;

	UPROPERTY(VisibleInstanceOnly)
	float CurrentPartHP;

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase;

};
