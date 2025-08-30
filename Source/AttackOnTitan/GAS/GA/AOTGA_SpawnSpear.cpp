// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_SpawnSpear.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/AOTWeapon_Spear.h"
#include "Character/BaseCharacter.h"
#include "CollisionQueryParams.h"
#include "Player/AOTEnemyDetectComponent.h"
#include "Engine/DamageEvents.h"

UAOTGA_SpawnSpear::UAOTGA_SpawnSpear()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAOTGA_SpawnSpear::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABaseCharacter* Player = Cast<ABaseCharacter>(ActorInfo->AvatarActor.Get());
	if (Player)
	{
		AActor* TargetActor = Player->GetEnemyDetectComponent()->GetCurrentActor();
		if (!TargetActor)
		{
			TargetActor = DetectEnemy(2000, 2000, true);
		}

		AController* Controller = Player->GetController();
		if (Controller)
		{
			FRotator CameraRotation = Controller->GetControlRotation();
			Player->SetActorRotation(FRotator(0.f, CameraRotation.Yaw, 0.f));

			// â ������ ����
			SpawnedSpear = Player->GetWorld()->SpawnActorDeferred<AAOTWeapon_Spear>(SpearClass, FTransform::Identity, Player, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

			// ĳ���� ����������� 60 ������ ��ġ �׸��� ������ ȸ���Ͽ� ����
			FTransform Transform = Player->GetTransform();
			FVector StartVec = Player->GetMesh()->GetSocketLocation("RightHandSocket") + Player->GetActorForwardVector() * 30;
			Transform.SetScale3D(FVector(0.5f, 0.5f, 0.7f));
			Transform.SetLocation(StartVec);
			Transform.SetRotation(FQuat(FRotator(-90.f, CameraRotation.Yaw, 0.f)));
			SpawnedSpear->SetTargetActor(TargetActor);
			SpawnedSpear->FinishSpawning(Transform);
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UAOTGA_SpawnSpear::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AActor* UAOTGA_SpawnSpear::DetectEnemy(float Range, float Radius, bool Single)
{
	AActor* Player = CurrentActorInfo->AvatarActor.Get();
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Detect), false, Player);

	FVector Start = Player->GetActorLocation();
	FVector End = Start + Player->GetActorForwardVector() * Range;

	AActor* ReturnActor = nullptr;

	bool HitDetected = Player->GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
	if (HitDetected)
	{
		float MinDistance = Radius * Radius;
		for (auto Result : OutHitResults)
		{
			FVector TargetActorLocation = Result.GetActor()->GetActorLocation();
			float Distance = FVector::Distance(TargetActorLocation, Player->GetActorLocation());
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ReturnActor = Result.GetActor();
			}
		}
	}

	/*#if ENABLE_DRAW_DEBUG
	
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = Range * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, Radius, FRotationMatrix::MakeFromZ(Player->GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
	
	#endif*/

	return ReturnActor;
}

