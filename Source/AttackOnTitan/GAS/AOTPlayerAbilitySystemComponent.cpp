// Unreal Engine Marketplace Copyright


#include "GAS/AOTPlayerAbilitySystemComponent.h"
#include "Data/AOTSkillData.h"

void UAOTPlayerAbilitySystemComponent::SetSkillData(int32 SkillSlot, UAOTSkillData* SkillData)
{
	if(SkillDatas.Contains(SkillSlot) == false)
		SkillDatas.Add(SkillSlot, SkillData);
}
