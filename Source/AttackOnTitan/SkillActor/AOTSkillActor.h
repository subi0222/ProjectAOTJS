// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbility.h"
#include "Enum.h"
#include "AOTSkillActor.generated.h"


class ACharacter;
class UAbilitySystemComponent;
class UAOTSkillData;
class UProjectileMovementComponent;
class UBoxComponent;
class UCapsuleComponent;
class UAOTSkillActorData;

UCLASS()
class ATTACKONTITAN_API AAOTSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAOTSkillActor();
	FORCEINLINE void SetTargetActor(AActor* TargetActor) { _TargetActor = TargetActor; }
	FORCEINLINE void SetSkillData(UAOTSkillData* SkillData) { _SkillData = SkillData; }
	FORCEINLINE void SetSkillActorData(UAOTSkillActorData* SkillActorData) { _SkillActorData = SkillActorData; }
	FORCEINLINE UBoxComponent* GetBoxComponent() { return BoxComp; }
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() {return CapsuleComp;}

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC;
	UPROPERTY()
	TObjectPtr<AActor> _TargetActor;
	UPROPERTY()
	TObjectPtr<UAOTSkillData> _SkillData;
	UPROPERTY()
	TObjectPtr<UAOTSkillActorData> _SkillActorData;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileComp;
};
