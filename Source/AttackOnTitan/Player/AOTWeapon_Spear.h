// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackOnTitan/AOTPlayerStat.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AOTWeapon_Spear.generated.h"

DECLARE_DELEGATE_OneParam(FOnCompleteDelegate,AActor*)

class UProjectileMovementComponent;
class UCapsuleComponent;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTWeapon_Spear : public AActor
{
	GENERATED_BODY()
public:
	AAOTWeapon_Spear();
	FORCEINLINE void SetInitialSpeed(float Speed) { ProjectileMovement->InitialSpeed = Speed; }
	FORCEINLINE void SetVelocity(FVector Velocity) { ProjectileMovement->Velocity = Velocity; }
	FORCEINLINE void SetTargetActor(AActor* _TargetActor) { TargetActor = _TargetActor; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, Category = WeaponMesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	FAOTPlayerStat WeaponStat;

	FOnCompleteDelegate OnOverlapDelegate;

private:
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = Particle)
	TObjectPtr<UParticleSystem> CollisionParticle;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;	

};
