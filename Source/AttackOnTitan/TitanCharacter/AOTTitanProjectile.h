// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/AOTTitanRangedAttackActorInterface.h"

#include "AOTTitanProjectile.generated.h"

DECLARE_DELEGATE_OneParam(FOnProjectileDestroy, AActor*)

class UGameplayEffect;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Blueprintable)
class ATTACKONTITAN_API AAOTTitanProjectile : public AActor, public IAOTTitanRangedAttackActorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAOTTitanProjectile();

	//RangedAttackActorInterface 구현부
	virtual void FollowTarget(ACharacter* Character) override;

	virtual bool HitCheck() override;

	FORCEINLINE virtual void SetDamage(float Damage) override {this->HitDamage = Damage;}

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleNormalThrowProjectileHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

public:

	FOnProjectileDestroy OnProjectileDestroy;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ProjectileMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> ProjectileEffectClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float GravityScale;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleInstanceOnly, Category = "Projectile")
	float HitDamage;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Torque;

};
