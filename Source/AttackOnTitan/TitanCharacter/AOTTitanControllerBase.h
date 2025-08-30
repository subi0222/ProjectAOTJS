// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AOTTitanBase.h"
#include "Components/AOTTitanRotatingComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "AOTTitanControllerBase.generated.h"

class UAISenseConfig_Damage;
class UAOTTitanRotatingComponent;
class AAOTTitanBase;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTTitanControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	explicit AAOTTitanControllerBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ToggleUpdateTarget(bool bActivate) { bUpdateTarget = bActivate; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ToggleActivateRotatingComp(bool bActivate) {RotatingComp->SetComponentTickEnabled(bActivate);}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void FocusToTarget() { SetFocus(CurrentTargetCharacter); }

	FORCEINLINE TObjectPtr<ACharacter> GetTargetCharacter() { return CurrentTargetCharacter; }

	FORCEINLINE bool HasTarget() const { return CurrentTargetCharacter != nullptr; }

	FORCEINLINE bool IsAttacking() const { return bAttacking; }

	FORCEINLINE bool HasPath() const { return TitanBase->HasPath(); }

	FORCEINLINE float GetTurningSpeed() const { return TurningSpeed; }

	void SetAttacking(bool bActivate);

	virtual void BeginPlay() override;

	float GetDistanceToPlayer();

	void UpdatePerceptionCompToTitan();

	void UpdateTarget();

protected:
	virtual void OnPossess(APawn* Titan) override;

private:
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);

	void SetupAIConfig();

protected:
	UPROPERTY(EditAnywhere, Category = Time)
	float UpdateTargetTime;

	UPROPERTY(EditAnywhere, Category = Time)
	float TurningToTargetTime;

	UPROPERTY(EditAnywhere, Category = Speed)
	float TurningSpeed;

private:
	UPROPERTY(EditDefaultsOnly, Category = AIBehavior, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAOTTitanRotatingComponent> RotatingComp;

	UPROPERTY(EditDefaultsOnly, Category = AIBehavior, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY()
	TObjectPtr<ACharacter> CurrentTargetCharacter;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

	UPROPERTY()
	TArray<TObjectPtr<ACharacter>> TargetList;

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> TitanBase;

	bool bTurnToTarget;

	bool bUpdateTarget;

	bool bAttacking;
};
