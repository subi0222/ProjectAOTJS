// Unreal Engine Marketplace Copyright


#include "BTTask_IncrementIndex.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanBase.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanControllerBase.h"
#include "AttackOnTitan/TitanCharacter/AOTTitanPath.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementIndex::UBTTask_IncrementIndex(FObjectInitializer const& ObjectInitializer)
	:UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Increment Index";
}

EBTNodeResult::Type UBTTask_IncrementIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if (auto* const TitanControllerBase = Cast<ATitanControllerBase>(OwnerComp.GetAIOwner())) {
	// 	if (auto* const AITitan = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn())) {
	// 		if (UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent()) {
	// 			uint8 MinIndex = 0;
	// 			uint8 MaxIndex = AITitan->GetTitanPath()->Num() - 1;
	// 			uint8 Index = BlackBoard->GetValueAsInt(TEXT("Index"));
	//
	// 			if (bDirectional) {
	// 				if (Index >= MaxIndex && DirectionType == EDirectionType::Forward) {
	// 					DirectionType = EDirectionType::Reverse;
	// 				}
	// 				else if (Index <= 0 && DirectionType == EDirectionType::Reverse) {
	// 					DirectionType = EDirectionType::Forward;
	// 				}
	// 			}
	// 			uint8 NextIndex = (DirectionType == EDirectionType::Forward ? Index + 1 : Index - 1);
	// 			if(NextIndex >= MinIndex && NextIndex <= MaxIndex)
	// 				BlackBoard->SetValueAsInt(TEXT("Index"), NextIndex);
	// 			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// 			return EBTNodeResult::Succeeded;
	// 		}
	// 	}
	// }
	return EBTNodeResult::Failed;
}
