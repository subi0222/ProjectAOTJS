// Unreal Engine Marketplace Copyright


#include "AI/BTTask_FindRandomLocation.h"

#include "AttackOnTitan.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectIntializer)
	: Super(ObjectIntializer)
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAOTTitanControllerBase*  TitanControllerBase = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner()))
	{
		if (AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn()))
		{
			if (TitanBase->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
			{
				return EBTNodeResult::Failed;
			}

			if (auto* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				const FVector Origin = TitanBase->GetActorLocation();
				const float Distance = FMath::Max(RandomRadius, 80.f);
				const float RandomAngle = FMath::FRandRange(0.f, 50.f);
				const FVector Direction = FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle)), FMath::Sin(FMath::DegreesToRadians(RandomAngle)), 0.f);

				FVector TargetLocation = Origin + Direction * Distance;
				TargetLocation.Z = Origin.Z; // 꼭 맞춰주세요


				const FVector NavExtent(150.f, 150.f, 500.f); // 넉넉한 범위
				FNavLocation ProjectedLocation;

				if (NavigationSystem->ProjectPointToNavigation(TargetLocation, ProjectedLocation, NavExtent))
				{
					// DrawDebugSphere(GetWorld(), TargetLocation, 60, 20, FColor::Blue, true, 2.0f);
					UE_LOG(LogTemp, Warning, TEXT("Projection Success: %s"), *ProjectedLocation.Location.ToString());
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ProjectedLocation.Location);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
				else
				{
					if (bTurning)
					{
						TargetLocation = Origin + -Direction * Distance;
						if (NavigationSystem->ProjectPointToNavigation(TargetLocation, ProjectedLocation, NavExtent))
						{
							// DrawDebugSphere(GetWorld(), TargetLocation, 60, 20, FColor::Blue, true, 2.0f);
							UE_LOG(LogTemp, Warning, TEXT("Projection Success: %s"), *ProjectedLocation.Location.ToString());
							OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ProjectedLocation.Location);
							FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
							return EBTNodeResult::Succeeded;
						}
					}
					UE_LOG(LogTemp, Error, TEXT("Projection Failed. Target: %s"), *TargetLocation.ToString());
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					return EBTNodeResult::Failed;
				}
			}
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}

void UBTTask_FindRandomLocation::OnInstanceCreated(UBehaviorTreeComponent& OwnerComp)
{
	Super::OnInstanceCreated(OwnerComp);
}
