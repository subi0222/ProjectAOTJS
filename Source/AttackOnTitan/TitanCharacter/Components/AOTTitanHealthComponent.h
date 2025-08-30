// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AOTTitanHealthComponent.generated.h"

class AAOTTitanBase;
class UAOTTitanSeveredPartComponent;
class UAOTTitanStatData;
class UAOTTitanAbilitySystemComponent;
class UAOTTitanHealthAttributeSet;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTitanHealthChanged, float, RemainHPRate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTitanDieStart);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ATTACKONTITAN_API UAOTTitanHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	 
public:	
	// Sets default values for this component's properties
	UAOTTitanHealthComponent();

	virtual void BeginPlay() override;

	void InitializeWithAbilitySystem(UAOTTitanAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable)
	void SeveredBodyPart(UAOTTitanSeveredPartComponent* BodyPart);

	UFUNCTION(BlueprintCallable)
	void SetDead(bool bActivate);

	UAnimMontage* GetRandomDieMontage() const;

	void TriggerDieEvent();

protected:

	void AddStamina();
	
public:
	
	FOnTitanHealthChanged OnTitanHealthChangedDelegate;

	//체력바와 컨트롤러 Unposess를 바인딩함.
	FOnTitanDieStart OnTitanDieStartDelegate;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> DieMontages;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> StaminaEffectClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float StaminaAddTime;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float StaminaAmount;

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase;

	UPROPERTY()
	TObjectPtr<const UAOTTitanHealthAttributeSet> TitanHealthAttributeSet;

	UPROPERTY()
	TObjectPtr<UAOTTitanAbilitySystemComponent> TitanASC;
	
};
