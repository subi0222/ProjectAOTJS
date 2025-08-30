// Unreal Engine Marketplace Copyright


#include "AI/BTTask_EscapeFromPlayer.h"

#include "AttackOnTitan.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/AOTTitanControllerBase.h"

EBTNodeResult::Type UBTTask_EscapeFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAOTTitanControllerBase* TitanControllerBase = Cast<AAOTTitanControllerBase>(OwnerComp.GetAIOwner()))
	{
		if (AAOTTitanBase* TitanBase = Cast<AAOTTitanBase>(TitanControllerBase->GetPawn()))
		{
			if (UCharacterMovementComponent* CharacterMovementComp = TitanBase->GetCharacterMovement())
			{
				CharacterMovementComp->SetMovementMode(MOVE_Walking);
				CharacterMovementComp->MaxWalkSpeed = EscapeSpeed;
				AOT_LOG(LogAOTAI, Log, TEXT("Speed : %f"), EscapeSpeed);
			}
			
			if (UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				// 전제: NavigationSystem, TitanBase, TitanControllerBase 사용 중
				// 필요 include: NavigationSystem.h, AIController.h, NavigationPath.h

				const FVector TitanLoc = TitanBase->GetActorLocation();

				// 1) 기준 방향: 타겟 있으면 타겟 Forward, 없으면 본인 Forward
				FVector BaseDir = FVector::ZeroVector;
				if (ACharacter* TargetChar = TitanControllerBase->GetTargetCharacter())
				{
					BaseDir = -(TargetChar->GetActorLocation() - TitanBase->GetActorLocation()).GetSafeNormal();
				}
				else
				{
					BaseDir = TitanBase->GetActorForwardVector();
				}
				BaseDir.Z = 0.f;
				if (!BaseDir.Normalize())
				{
					BaseDir = TitanBase->GetActorForwardVector().GetSafeNormal2D();
				}

				// 2) 파라미터
				const float AngleStepDeg = 15.f; // 한 스텝 회전 각
				const int32 MaxStepsEachSide = 8; // 좌/우 최대 시도 횟수
				const FVector NavExtent(150.f, 150.f, 500.f);
				
				
				FNavLocation ProjectLocation;

				for (int i = 0; i < MaxStepsEachSide; i++)
				{
					const float Angle = i * AngleStepDeg;
					// BaseDir을 기준으로 Yaw(+Angle) 회전
					FVector RightRotatedDir = FRotator(0.f, Angle, 0.f).RotateVector(BaseDir);

					// 정규화 (안정성 위해)
					RightRotatedDir = RightRotatedDir.GetSafeNormal2D();

					// EscapeDistance만큼 전방 지점 구하기
					FVector CandidateLocation = TitanLoc + RightRotatedDir * EscapeDistance;
					if (NavigationSystem->ProjectPointToNavigation(CandidateLocation, ProjectLocation, NavExtent))
					{
						DrawDebugSphere(GetWorld(), ProjectLocation.Location, 80, 6, FColor::Green, false, 3.0f);
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocation.SelectedKeyName, ProjectLocation.Location);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
					DrawDebugSphere(GetWorld(), ProjectLocation.Location, 80, 6, FColor::Red, false, 3.0f);
				}

				for (int i = 0; i < MaxStepsEachSide; i++)
				{
					const float Angle = i * AngleStepDeg;
					// BaseDir을 기준으로 Yaw(+Angle) 회전
					FVector LeftRotatedDir = FRotator(0.f, -Angle, 0.f).RotateVector(BaseDir);

					// 정규화 (안정성 위해)
					LeftRotatedDir = LeftRotatedDir.GetSafeNormal2D();

					// EscapeDistance만큼 전방 지점 구하기
					FVector CandidateLocation = TitanLoc + LeftRotatedDir * EscapeDistance;
					if (NavigationSystem->ProjectPointToNavigation(CandidateLocation, ProjectLocation, NavExtent))
					{
						DrawDebugSphere(GetWorld(), ProjectLocation.Location, 80, 6, FColor::Green, false, 3.0f);
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocation.SelectedKeyName, ProjectLocation.Location);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
					DrawDebugSphere(GetWorld(), ProjectLocation.Location, 80, 6, FColor::Red, false, 3.0f);
				}
				
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
