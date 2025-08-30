// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enum.h"
#include "AOTSkillActorData.generated.h"

class AAOTSkillActor;
class UGameplayEffect;
class UParticleSystem;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTSkillActorData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAOTSkillActor> SkillActorClass;
	// �������� �߻�ü����
	UPROPERTY(EditAnywhere)
	SkillActorType _ActorType;
	// �����ڿ� �ٿ���, ������ �ֺ���, ��󿡽����Ұ���
	UPROPERTY(EditAnywhere)
	SpawnType _SpawnType;
	UPROPERTY(EditAnywhere)
	SkillActorShape _Shape;
	UPROPERTY(EditAnywhere)
	FVector CollisionSize;
	UPROPERTY(EditAnywhere)
	float Duration;
	UPROPERTY(EditAnywhere)
	FName SpawnSocketName;
	UPROPERTY(EditAnywhere)
	FName CollisionName;
	// Target�� �ƴѳ������� �̰��� �ؿ� Rotation���� �����ؼ� �����Ѵ�.
	UPROPERTY(EditAnywhere)
	FRotator SpawnRotation;
	// �ϴ� Target���� �����ϴ� ���ʹ� �̰����� �����Ͽ� �����Ѵ�.
	UPROPERTY(EditAnywhere)
	FVector SpawnOffset;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> TargetEffects;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> Particle;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> NiagaraParticle;
	UPROPERTY(EditAnywhere, Category = Area)
	float EffectApplyInterval;
};
