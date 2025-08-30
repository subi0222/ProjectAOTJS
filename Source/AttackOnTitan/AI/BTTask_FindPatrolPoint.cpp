// Unreal Engine Marketplace Copyright


#include "BTTask_FindPatrolPoint.h"

#include "AttackOnTitan.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanBase.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanPath.h"
#include "GameFrameWork/CharacterMovementComponent.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint(FObjectInitializer const& ObjectInitializer)
	: UBTTask_BlackboardBase()
{
	NodeName = "FindPatrolPoint";
	
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const AAOTTitanControllerBase* TitanControllerBase = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner())) {
		if (const AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn())) {

			//속도 정하고
			UCharacterMovementComponent* CharacterMovementComp = TitanBase->GetCharacterMovement();
			if (CharacterMovementComp)
			{
				CharacterMovementComp->SetMovementMode(MOVE_Walking);
				CharacterMovementComp->MaxWalkSpeed = Speed;
			}
			
			auto* BlackboardComp = OwnerComp.GetBlackboardComponent();
			if (AAOTTitanPath* TitanPath = TitanBase->GetTitanPath()) {
				FVector NextPatrolPointPosition = TitanPath->GetNextPatrolPoint();
				AOT_LOG(LogAOT, Log, TEXT("Patrol Position Point : %s"), *NextPatrolPointPosition.ToString());
				BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(), NextPatrolPointPosition);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
