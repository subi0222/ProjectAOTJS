// Unreal Engine Marketplace Copyright


#include "BTTask_FindPlayerLocation.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "TitanCharacter/AOTTitanBase.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Player Location";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get target location from  blackboard via NPC's controller
	// get player character
	if (auto* const TitanControllerBase = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner())) {
		if (AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn()))
		{
			//속도 정하고
			UCharacterMovementComponent* CharacterMovementComp = TitanBase->GetCharacterMovement();
			if (CharacterMovementComp)
			{
				CharacterMovementComp->MaxWalkSpeed = Speed;
			}
		
			// get player location to use ase a origin 
			if (ACharacter* Player = TitanControllerBase->GetTargetCharacter()) {
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
