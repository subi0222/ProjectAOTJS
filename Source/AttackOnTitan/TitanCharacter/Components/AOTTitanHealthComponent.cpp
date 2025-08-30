// Unreal Engine Marketplace Copyright


#include "TitanCharacter/Components/AOTTitanHealthComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AOTGameplayTag.h"
#include "AOTTitanSeveredPartComponent.h"
#include "AttackOnTitan.h"
#include "Components/CapsuleComponent.h"
#include "TitanCharacter/GAS/AOTTitanAbilitySystemComponent.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanHealthAttributeSet.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/UI/AOTTitanHPWidgetComponent.h"

UAOTTitanHealthComponent::UAOTTitanHealthComponent()
{
}

void UAOTTitanHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TitanAddStaminaTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TitanAddStaminaTimerHandle, 
		this,
		&UAOTTitanHealthComponent::AddStamina,
		StaminaAddTime, true);
}

void UAOTTitanHealthComponent::InitializeWithAbilitySystem(UAOTTitanAbilitySystemComponent* InASC)
{
	if (InASC)
	{
		TitanASC = InASC;
		TitanBase = TitanASC->GetTitanOwner();
		TitanHealthAttributeSet = TitanASC->GetSet<UAOTTitanHealthAttributeSet>();
		check(TitanHealthAttributeSet);
	} 
}

void UAOTTitanHealthComponent::SeveredBodyPart(UAOTTitanSeveredPartComponent* BodyPart)
{
	if (!TitanBase)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanBase Is Null"))
		return;
	}
	USkeletalMeshComponent* SkeletalMeshComp = TitanBase->GetMesh();
	check(SkeletalMeshComp);
	
	const USkeletalMeshSocket* Socket =
		SkeletalMeshComp->GetSocketByName(BodyPart->GetAttachSocketName());

	if (!Socket)
	{
		AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("No Socket In Mesh Comp"))
		return;
	}
	FName BoneName = Socket->BoneName;
	SkeletalMeshComp->HideBoneByName(BoneName, PBO_Term);
}

void UAOTTitanHealthComponent::SetDead(bool bActivate)
{
	if (bActivate)
	{
		if (!TitanBase)
		{
			AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanBase Is Null"))
			return;
		}
	
		if (!TitanASC)
		{
			AOT_LOG(LogAOT, Warning, TEXT("%s"), TEXT("TitanASC Is Null"))
			return;
		}

		//HP Bar 없애줌
		UAOTTitanHPWidgetComponent* HPWidgetComp =
			TitanBase->GetComponentByClass<UAOTTitanHPWidgetComponent>();
		if (HPWidgetComp)
		{
			HPWidgetComp->SetVisibility(false);
		}

		//태그 추가
		TitanASC->AddLooseGameplayTag(Titan_State_Dead);

		//움직임 없음
		TitanBase->GetCharacterMovement()->DisableMovement();

		//래그돌 활성화
 		TitanBase->GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
		TitanBase->GetMesh()->SetSimulatePhysics(true);        // 래그돌 활성화
		TitanBase->GetMesh()->SetCollisionProfileName(TEXT("Ragdoll")); // 충돌 프로필
	}
}

UAnimMontage* UAOTTitanHealthComponent::GetRandomDieMontage() const
{
	if (DieMontages.Num() == 0)
	{
		AOT_LOG(LogAOT, Warning, TEXT("DieMontage Is Null"));
		return nullptr;
	}
	int8 RandIndex = FMath::RandRange(0, DieMontages.Num() - 1);
	return DieMontages[RandIndex];
}

void UAOTTitanHealthComponent::TriggerDieEvent()
{
	if (!TitanASC)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("TitanASC Is Null"))
		return;
	}
	FGameplayEventData EventData;
	EventData.OptionalObject = this;
	EventData.EventTag = Titan_Event_StartDying;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TitanBase, Titan_Event_StartDying,EventData);
}

void UAOTTitanHealthComponent::AddStamina()
{
	if (!StaminaEffectClass)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("StaminaEffect Is Null"))
		return;
	}

	
	FGameplayEffectContextHandle EffectContextHandle = TitanASC->MakeEffectContext();
	FGameplayEffectSpecHandle StaminaEffectSpecHandle =
		TitanASC->MakeOutgoingSpec(StaminaEffectClass,
		1.0f,
		EffectContextHandle);
	StaminaEffectSpecHandle.Data->SetSetByCallerMagnitude(Titan_Effect_Stamina, StaminaAmount);
	TitanASC->ApplyGameplayEffectSpecToSelf(*StaminaEffectSpecHandle.Data);
}
