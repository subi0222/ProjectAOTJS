// Unreal Engine Marketplace Copyright


#include "BTService_UpdateTitanTargetHeight.h"

#include "AbilitySystemComponent.h"
#include "AOTGameplayTag.h"
#include "AttackOnTitan.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanBase.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_UpdateTitanTargetHeight::UBTService_UpdateTitanTargetHeight()
{
	bNotifyBecomeRelevant = true;
	NodeName = "Update Titan Target Height";
}

void UBTService_UpdateTitanTargetHeight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAOTTitanControllerBase* TCB = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner());
	if (!TCB)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("TitanControllerBase Is Null"))
		return;
	}
	
	AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TCB->GetPawn());
	if (!TitanBase)
	{
		AOT_LOG(LogAOTAI, Warning, TEXT("%s"), TEXT("For Controller, TitanBase Is Null"))
		return;
	}
	FGameplayTagContainer TitanOwnedTagContainer;
	

	if (!TCB->GetTargetCharacter())
	{
		AOT_LOG(LogAOTAI, Log, TEXT("%s"), TEXT("Titan Doesn't Have Target Yet"))
		return;
	}
	UpdateTargetLocationTag(TitanBase, TCB->GetTargetCharacter()->GetActorLocation().Z);
}

bool UBTService_UpdateTitanTargetHeight::UpdateTargetLocationTag(AAOTTitanBase* TitanBase, float TargetZLocation)
{
	FGameplayTag TargetLocationTag = GetTargetTag(TitanBase, TargetZLocation);
	UAbilitySystemComponent* ASC = TitanBase->GetAbilitySystemComponent();
	if (!ASC)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("TitanBase Does Not Have ASC"));
		return false;
	}

	//만약 TargetLocationTag 관련 태그가 없다면 넣고 끝
	if (!ASC->HasMatchingGameplayTag(Titan_State_TargetLocation))
	{
		ASC->AddLooseGameplayTag(TargetLocationTag);
		return true;
	}

	//갖고 있다면 찾은 다음
	FGameplayTag BeforeTargetLocationTag;
	for (FGameplayTag TitanTag : ASC->GetOwnedGameplayTags())
	{
		if (TitanTag.MatchesTag(Titan_State_TargetLocation))
		{
			BeforeTargetLocationTag = TitanTag;
			break;
		}
	}

	//같다면 그냥 냅두고
	if (BeforeTargetLocationTag == TargetLocationTag)
	{
		return true;
	}

	// 다르다면 갱신
	if (BeforeTargetLocationTag != TargetLocationTag)
	{
		ASC->RemoveLooseGameplayTag(BeforeTargetLocationTag);
		ASC->AddLooseGameplayTag(TargetLocationTag);
		return true;
	}
	return false;
}

FGameplayTag UBTService_UpdateTitanTargetHeight::GetTargetTag(AAOTTitanBase* TitanBase, float TargetZLocation)
{
	FBox Box = TitanBase->GetMesh()->Bounds.GetBox();
	float Bottom = Box.Min.Z;
	float Top = Box.Max.Z;
	float Middle = Bottom + Top / 2.0;
	if (TargetZLocation <= Middle)
		return Titan_State_TargetLocation_Bottom;
	else if (TargetZLocation > Top)
		return Titan_State_TargetLocation_Top;
	else
		return Titan_State_TargetLocation_Middle;
}

