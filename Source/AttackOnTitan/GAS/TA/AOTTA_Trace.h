// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "AOTTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	AAOTTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

	FORCEINLINE void SetSkillNum(int32 _SkillNum) { SkillNum = _SkillNum; }
	FORCEINLINE void SetTraceMonster(bool Flag) { bTraceMonster = Flag; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
	int32 SkillNum;

	bool bShowDebug = true;
	bool bTraceMonster = true;

};
