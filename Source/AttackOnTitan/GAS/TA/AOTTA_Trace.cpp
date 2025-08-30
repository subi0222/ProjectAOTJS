// Unreal Engine Marketplace Copyright


#include "GAS/TA/AOTTA_Trace.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GAS/AttributeSet/AOTPlayerAttributeSet.h"
#include "GAS/AOTPlayerAbilitySystemComponent.h"
#include "Data/AOTSkillData.h"


AAOTTA_Trace::AAOTTA_Trace()
{
}

void AAOTTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AAOTTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AAOTTA_Trace::MakeTargetData() const
{
		ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAOTPlayerAbilitySystemComponent* ASC = Cast<UAOTPlayerAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor));
	if (!ASC)
	{
		//ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	const UAOTPlayerAttributeSet* AttributeSet = ASC->GetSet<UAOTPlayerAttributeSet>();
	if (!AttributeSet)
	{
		//ABGAS_LOG(LogABGAS, Error, TEXT("ABCharacterAttributeSet not found!"));
		return FGameplayAbilityTargetDataHandle();
	}

	FHitResult OutHitResult;
	float AttackRange;
	float AttackRadius;

	UAOTSkillData* SkillData = ASC->GetSkillData(SkillNum);
	if (SkillNum == 0)
	{
		AttackRange = AttributeSet->GetAttackRange();
		AttackRadius = AttributeSet->GetAttackRadius();
	}
	else
	{
		AttackRange = SkillData->SkillRange;
		AttackRadius = SkillData->SkillRadius;
	}

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FRotator NewRotation = FRotator(0.f,Character->GetActorRotation().Yaw,0.f);
	const FVector NewVector = NewRotation.Vector();
	const FVector Start = Character->GetActorLocation() + NewVector * (Character->GetCapsuleComponent()->GetScaledCapsuleRadius() + SkillData->ForwardOffset) + SkillData->ZOffset;
	const FVector End = Start + NewVector * AttackRange;

	bool HitDetected;
	if(bTraceMonster)
		HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel5, FCollisionShape::MakeSphere(AttackRadius), Params);
	else
		HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel5, FCollisionShape::MakeSphere(AttackRadius), Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG

	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(NewVector).ToQuat(), DrawColor, false, 5.0f);
	}

#endif

	return DataHandle;
}
