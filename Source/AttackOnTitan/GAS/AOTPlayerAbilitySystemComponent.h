// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AOTPlayerAbilitySystemComponent.generated.h"

class UAOTSkillData;
/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTPlayerAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE UAOTSkillData* GetSkillData(int32 SkillSlot) { return SkillDatas[SkillSlot]; }

	void SetSkillData(int32 SkillSlot, UAOTSkillData* SkillData);	

protected:
	UPROPERTY()
	TMap<int32, TObjectPtr<UAOTSkillData>> SkillDatas;
	
};
