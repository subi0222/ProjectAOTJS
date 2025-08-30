// Unreal Engine Marketplace Copyright


#include "GAS/AT/AOTAT_Jump.h"
#include "GameFramework/Character.h"

UAOTAT_Jump::UAOTAT_Jump()
{
}

UAOTAT_Jump* UAOTAT_Jump::CreateTask(UGameplayAbility* OwningAbility)
{
	UAOTAT_Jump* NewTask = NewAbilityTask<UAOTAT_Jump>(OwningAbility);
	return NewTask;
}

void UAOTAT_Jump::Activate()
{
	Super::Activate();
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UAOTAT_Jump::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UAOTAT_Jump::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UAOTAT_Jump::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UAOTAT_Jump::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
