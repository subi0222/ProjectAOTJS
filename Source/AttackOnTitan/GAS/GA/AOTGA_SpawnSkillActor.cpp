// Unreal Engine Marketplace Copyright


#include "GAS/GA/AOTGA_SpawnSkillActor.h"
#include "Data/AOTSkillData.h"
#include "Data/AOTSkillActorData.h"
#include "SkillActor/AOTSkillActor.h"
#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Player/AOTEnemyDetectComponent.h"
#include "Enum.h"

UAOTGA_SpawnSkillActor::UAOTGA_SpawnSkillActor()
{
}

void UAOTGA_SpawnSkillActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	int32 SkillActorNum = TriggerEventData->EventMagnitude;	
	UAOTSkillData* SkillData = const_cast<UAOTSkillData*>(Cast<UAOTSkillData>(TriggerEventData->OptionalObject));
	UAOTSkillActorData* SkillActorData = SkillData->SkillActorData;
	FVector SpawnOffset = FVector::ZeroVector;
	if (SkillData->SkillActorOffsets.Num() > SkillActorNum)
	{
		SpawnOffset = SkillData->SkillActorOffsets[SkillActorNum];
	}
	ABaseCharacter* Character = Cast<ABaseCharacter>(ActorInfo->AvatarActor);

	SpawnedSkillActor = Character->GetWorld()->SpawnActorDeferred<AAOTSkillActor>(SkillActorData->SkillActorClass, FTransform::Identity
		, Character, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	SpawnedSkillActor->SetSkillData(SkillData);
	SpawnedSkillActor->SetSkillActorData(SkillActorData);

	FTransform NewTransform = Character->GetActorTransform();

	switch (SkillActorData->_SpawnType)
	{
	case SpawnType::SpawnGround:
	{
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Character);
		FVector Start = Character->GetActorLocation();
		FVector End = Start - FVector(0.0f, 0.0f, 500.f);
		bool Result = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel3, Params);
		if (Result)
		{
			FVector HitPoint = HitResult.ImpactPoint;
			NewTransform.SetLocation(HitPoint);
		}

		FRotator NewRotation = Character->GetActorRotation();
		FVector ForwardVec = Character->GetActorForwardVector();
		FVector RightVec = Character->GetActorRightVector();
		FVector NewLocation = NewTransform.GetLocation() + ForwardVec * SpawnOffset.X + RightVec * SpawnOffset.Y;
		NewLocation.Z += SpawnOffset.Z;
		NewTransform.SetLocation(NewLocation);
		NewRotation.Yaw += SkillActorData->SpawnRotation.Yaw;
		NewTransform.SetRotation(FQuat(NewRotation));
		SpawnedSkillActor->FinishSpawning(NewTransform);
		SpawnedSkillActor->SetLifeSpan(SkillActorData->Duration);
	}
	break;
	case SpawnType::CasterAround:
	{
		FRotator NewRotation = Character->GetActorRotation();
		FVector ForwardVec = Character->GetActorForwardVector();
		FVector RightVec = Character->GetActorRightVector();
		FVector NewLocation = NewTransform.GetLocation() + ForwardVec * SpawnOffset.X + RightVec * SpawnOffset.Y;
		NewLocation.Z += SpawnOffset.Z;
		NewTransform.SetLocation(NewLocation);
		NewRotation.Yaw += SkillActorData->SpawnRotation.Yaw;
		NewTransform.SetRotation(FQuat(NewRotation));
		SpawnedSkillActor->FinishSpawning(NewTransform);
		SpawnedSkillActor->SetLifeSpan(SkillActorData->Duration);
	}
	break;
	case SpawnType::CasterAttach:
	{
		FRotator NewRotation = Character->GetActorRotation();
		FVector ForwardVec = Character->GetActorForwardVector();
		FVector RightVec = Character->GetActorRightVector();
		FVector NewLocation = Character->GetActorLocation() + ForwardVec * SpawnOffset.X + RightVec * SpawnOffset.Y;
		NewLocation.Z += SpawnOffset.Z;
		NewTransform.SetLocation(NewLocation);
		NewTransform.SetRotation(NewTransform.GetRotation() * FQuat(SkillActorData->SpawnRotation));
		SpawnedSkillActor->FinishSpawning(Character->GetTransform());
		SpawnedSkillActor->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SpawnedSkillActor->SetLifeSpan(SkillActorData->Duration);
	}
	break;
		// 여기는 보통 Projectile SkillActor에서 감지된 Actor가있을때 그 Actor를 설정해준다.
	case SpawnType::Target:
	{
		FVector NewLocation = NewTransform.GetLocation();
		if (SkillActorData->SpawnSocketName != NAME_None)
			NewTransform = Character->GetMesh()->GetSocketTransform(SkillActorData->SpawnSocketName);

		FVector ForwardVec = Character->GetActorForwardVector();
		FVector RightVec = Character->GetActorRightVector();
		NewLocation  += ForwardVec * SpawnOffset.X + RightVec * SpawnOffset.Y;
		NewLocation.Z += SpawnOffset.Z;
		NewTransform.SetLocation(NewLocation);
		SpawnedSkillActor->SetTargetActor(Character->GetEnemyDetectComponent()->GetCurrentActor());
		SpawnedSkillActor->FinishSpawning(NewTransform);
		SpawnedSkillActor->SetLifeSpan(SkillActorData->Duration);
	}
	break;
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UAOTGA_SpawnSkillActor::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
