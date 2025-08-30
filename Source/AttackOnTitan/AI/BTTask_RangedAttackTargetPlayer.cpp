// Unreal Engine Marketplace Copyright


#include "BTTask_RangedAttackTargetPlayer.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_RangedAttackTargetPlayer::UBTTask_RangedAttackTargetPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Ranged Attack Target Player";
}

EBTNodeResult::Type UBTTask_RangedAttackTargetPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if (auto* const TitanControllerBase = Cast<ATitanControllerBase>(OwnerComp.GetAIOwner())) {
	// 	if (auto* const TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn()))
	// 	{
	// 		TitanBase->RotateToTarget();
	// 		if (!TitanBase->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()){
	// 			TitanControllerBase->StopMovement();
	// 			TitanBase->TrySpecialAttack();
	// 			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsAttacking"), true);
	// 			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 			return EBTNodeResult::Succeeded;
	// 		}
	// 	}
	// }
	return EBTNodeResult::Failed;
}
