// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AOTTitanBase.h"
#include "GameFramework/Actor.h"
#include "AOTTitanEnergyEmission.generated.h"


class UWidgetComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UGameplayEffect;
class UCapsuleComponent;
class AAOTTitanBase;

UCLASS()
class ATTACKONTITAN_API AAOTTitanEnergyEmission : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAOTTitanEnergyEmission();

	void SetEnergyEmission(AAOTTitanBase* TitanBase, float EmissionRange, float EnergyPerTime);

	void ActivateEmission();

	void DeactivateEmission();

protected:
	
	UFUNCTION()
	void HandleEmissionHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

private:

	void FollowTarget(float DeltaTime);

	void FollowTitanOwner(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetFollowTarget(ACharacter* TargetCharacter, float Speed);

	void SetFollowTitanOwner();

	void StopFollowTarget();


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ValidAttackPosition", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> EmissionAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> EmissionEffect;

	UPROPERTY(EditdefaultsOnly, BlueprintReadWrite, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> EmissionWidget;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	float DamagePerTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	float Range;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	bool bFollowTarget;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	bool bFollowTitan;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Emission", meta = (AllowPrivateAccess = "true"))
	float FollowSpeed;

	UPROPERTY()
	TObjectPtr<ACharacter> FollowTargetCharacter;

	UPROPERTY()
	TObjectPtr<AAOTTitanBase> EmissionOwnerTitan;

};
