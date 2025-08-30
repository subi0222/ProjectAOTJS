// Unreal Engine Marketplace Copyright


#include "AOTBasePlayer.h"
#include "EnhancedInputComponent.h"
#include "Animation/AnimMontage.h"
#include "AttackOnTitan/Data/AOTBaseComboAttackData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttackOnTitan/Data/AOTSkillData.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Player/AOTEnemyDetectComponent.h"


AAOTBasePlayer::AAOTBasePlayer()
{
}

void AAOTBasePlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AAOTBasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	/*UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent >(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(IA_Skill_02, ETriggerEvent::Started, this, &AAOTBasePlayer::ActivateSkill, 1);
	}*/
}

void AAOTBasePlayer::ActivateSkill(int32 InputId)
{
	/*if (StatComponent->GetCurrentEnergy() < SkillDatas[InputId]->Energy)
	{
		UE_LOG(LogTemp, Error, TEXT("No Energy"));
		return;
	}*/

	//if (bSkilling == true)
	//	return;

	//if (SkillDatas[InputId]->bAir && !GetCharacterMovement()->IsFalling())
	//	return;

	//if (SkillTimerHandles[InputId].IsValid())
	//	return;
	//AActor* CurrentTargetActor = GetEnemyDetectComponent()->GetCurrentActor();
	//if (CurrentTargetActor == nullptr && FVector::Dist(CurrentTargetActor->GetActorLocation(), GetActorLocation()) > 800.f)
	//	return;

	//FRotator NewRotator = FVector(CurrentTargetActor->GetActorLocation() - GetActorLocation()).Rotation();
	//SetActorRotation(NewRotator);

	//bSkilling = true;
	//
	//CurrentInputId = InputId;

	//if (SkillDatas[InputId]->bMoving == false)
	//{
	//	if (SkillDatas[InputId]->bAir)
	//	{
	//		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	//		GetController()->SetIgnoreMoveInput(true);
	//	}
	//	else
	//		GetCharacterMovement()->SetGroundMovementMode(EMovementMode::MOVE_None);
	//}

	//FOnMontageEnded EndMontageDelegate;
	//EndMontageDelegate.BindUObject(this, &AAOTBasePlayer::EndSkill);
	////StatComponent->SetEnergy(StatComponent->GetCurrentEnergy() - SkillDatas[InputId]->Energy);
	//GetMesh()->GetAnimInstance()->Montage_Play(SkillDatas[InputId]->SkillMontage);
	//GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndMontageDelegate);
	//FTimerDelegate TimerDelegate;
	//TimerDelegate.BindUObject(this,&AAOTBasePlayer::EndSkillTimer, InputId);
	//GetWorld()->GetTimerManager().SetTimer(SkillTimerHandles[InputId], TimerDelegate, SkillDatas[InputId]->CoolTime, false);
}

//void AAOTBasePlayer::SkillHitCheck()
//{
//	if (CurrentInputId == -1)
//		return;
//	TArray<FHitResult> OutHitResults;
//	FHitResult OutHitResult;
//	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
//
//	const float AttackRange = SkillDatas[CurrentInputId]->Range;
//	const float AttackRadius = SkillDatas[CurrentInputId]->Radius;
//	const float AttackDamage = SkillDatas[CurrentInputId]->Damage;
//	
//	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
//	const FVector End = Start + GetActorForwardVector() * AttackRange;
//	bool HitDetected;
//	if (SkillDatas[CurrentInputId]->Single)
//	{
//		HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), Params);
//		if (HitDetected)
//		{
//			FDamageEvent DamageEvent;
//			OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
//		}
//	}
//	else
//	{
//		HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), Params);
//		if (HitDetected)
//		{
//			FDamageEvent DamageEvent;
//			for (const FHitResult& HitResult : OutHitResults)
//				HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
//		}
//	}
//
//
//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5f; // �յ� ũ��
//	float CapsuleRadius = AttackRadius; // ����(Radius) ũ��
//
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	// ĸ���� X�� �������� ȸ���Ͽ� "�յ� ����"���� ��� ����
//	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, CapsuleRadius, CapsuleRotation, DrawColor, false, 5.0f);
//
//#endif
//}

void AAOTBasePlayer::EndSkill(UAnimMontage* Montage, bool IsInterrupted)
{
	/*if (CurrentInputId == -1)
		return;
	if (SkillDatas[CurrentInputId]->bMoving == false)
	{
		if (SkillDatas[CurrentInputId]->bAir)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			GetController()->SetIgnoreMoveInput(false);
		}
		else
			GetCharacterMovement()->SetGroundMovementMode(EMovementMode::MOVE_Walking);
	}
	bSkilling = false;
	CurrentInputId = -1;*/
}

void AAOTBasePlayer::EndSkillTimer(int32 Index)
{
	//SkillTimerHandles[Index].Invalidate();
}