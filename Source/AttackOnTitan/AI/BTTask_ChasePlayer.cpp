// Unreal Engine Marketplace Copyright


#include "BTTask_ChasePlayer.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanBase.h"
#include "GameFrameWork/CharacterMovementComponent.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get target location from  blackboard via NPC's controller
	if (auto* const TitanControllerBase = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner())) {
		FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		if (auto* const TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn())) {

			if (!TitanBase->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) {
				TitanBase->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(TitanControllerBase, TargetLocation);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return EBTNodeResult::Failed;
			}
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
