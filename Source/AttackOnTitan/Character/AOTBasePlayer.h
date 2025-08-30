// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AOTBasePlayer.generated.h"

class UAOTBaseComboAttackData;
class UAnimMontage;
class UInputAction;
class UAOTSkillData;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTBasePlayer : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AAOTBasePlayer();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ActivateSkill(int32 InputId);
	virtual void EndSkill(UAnimMontage* Montage, bool IsInterrupted);
	virtual void EndSkillTimer(int32 Index);

};
