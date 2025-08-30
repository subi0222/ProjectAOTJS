// Unreal Engine Marketplace Copyright


#include "AOTAT_TitanSpawnActorTask.h"

#include "AttackOnTitan.h"
#include "GameFramework/Character.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/Interface/AOTTitanRangedAttackActorInterface.h"

UAOTAT_TitanSpawnActorTask* UAOTAT_TitanSpawnActorTask::CreateSpawnActorTask(UGameplayAbility* OwningAbility,
                                                                             TArray<FSpawnActorInformation>
                                                                             SpawnActorInformations,
                                                                             ACharacter* Target)
{
	UAOTAT_TitanSpawnActorTask* NewSpawnActorTask = NewAbilityTask<UAOTAT_TitanSpawnActorTask>(OwningAbility);
	NewSpawnActorTask->Target = Target;
	NewSpawnActorTask->SpawnActorInformations = SpawnActorInformations;
	return NewSpawnActorTask;
}

void UAOTAT_TitanSpawnActorTask::Activate()
{
	Super::Activate();
	for (auto SpawnActorInformation : SpawnActorInformations)
	{
		SpawnActor(SpawnActorInformation);
	}
	EndTask();
}

void UAOTAT_TitanSpawnActorTask::SpawnActor(FSpawnActorInformation SpawnActorInformation)
{
	AAOTTitanBase* TaskOwnTitan = Cast<AAOTTitanBase>(GetAvatarActor());
	ESpawnLocationType SpawnLocationType = SpawnActorInformation.SpawnLocationType;
	FName SocketName = SpawnActorInformation.SpawnSocketName;
	int SpawnActorCount = SpawnActorInformation.SpawnActorNum;
	AOT_LOG(LogAOTGAS, Log, TEXT("Socket Name : %s"), *SpawnActorInformation.SpawnSocketName.ToString());
	for (int i = 0; i < SpawnActorCount; i++)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Ability->GetAvatarActorFromActorInfo();
		AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(SpawnActorInformation.SpawnActorClass, SpawnParameters);
		if (SpawnActor)
		{
			if (IAOTTitanRangedAttackActorInterface* Interface = Cast<IAOTTitanRangedAttackActorInterface>(SpawnActor))
			{
				Interface->SetDamage(SpawnActorInformation.Damage);
			}
			if (TaskOwnTitan)
			{
				TaskOwnTitan->AddSpawnActor(SpawnActor);
				if (SpawnLocationType == ESpawnLocationType::Target)
				{
					SpawnActor->AttachToComponent(Target->GetMesh(),
					                              FAttachmentTransformRules::SnapToTargetIncludingScale,
					                              SocketName);
					SpawnActor->SetActorScale3D(SpawnActorInformation.SpawnActorScale * FVector3d::One());
					return;
				}
				SpawnActor->AttachToComponent(TaskOwnTitan->GetMesh(),
				                              FAttachmentTransformRules::SnapToTargetIncludingScale,
				                              SocketName);
				SpawnActor->SetActorScale3D(SpawnActorInformation.SpawnActorScale * FVector3d::One());
			}
		}
	}
}
