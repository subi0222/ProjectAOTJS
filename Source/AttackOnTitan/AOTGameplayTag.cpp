#include "AOTGameplayTag.h"
#include "GameplayTagsManager.h"

FAOTGameplayTags FAOTGameplayTags::GameplayTags;

/*
* Define State Tags
*/
UE_DEFINE_GAMEPLAY_TAG(Titan_State, "Titan.State");

UE_DEFINE_GAMEPLAY_TAG(Titan_State_Attacking, "Titan.State.Attacking");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Dead, "Titan.State.Dead");

UE_DEFINE_GAMEPLAY_TAG(Titan_State_Neck_Alive, "Titan.State.Neck.Alive");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Neck_Severed, "Titan.State.Neck.Severed");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Arm_TwoArm, "Titan.State.Arm.TwoArm");

UE_DEFINE_GAMEPLAY_TAG(Titan_State_Arm_OneArm, "Titan.State.Arm.OneArm");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Arm_OneArm_LeftArm, "Titan.State.Arm.OneArm.LeftArm");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Arm_OneArm_RightArm, "Titan.State.Arm.OneArm.RightArm");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Leg_TwoLeg, "Titan.State.Leg.TwoLeg");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Leg_OneLeg, "Titan.State.Leg.OneLeg");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Leg_OneLeg_LefLeg, "Titan.State.Leg.OneLeg.LeftLeg");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_Leg_OneLeg_RightLeg, "Titan.State.Leg.OneLeg.RightLeg");

UE_DEFINE_GAMEPLAY_TAG(Titan_State_TargetLocation, "Titan.State.TargetLocation");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_TargetLocation_Top, "Titan.State.TargetLocation.Top");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_TargetLocation_Middle, "Titan.State.TargetLocation.Middle");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_TargetLocation_Bottom, "Titan.State.TargetLocation.Bottom");

UE_DEFINE_GAMEPLAY_TAG(Titan_State_TitanBodyTarget, "Titan.State.TitanBodyTarget");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_TitanBodyTarget_LeftArm, "Titan.State.TitanBodyTarget.LeftArm");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_TitanBodyTarget_RightArm, "Titan.State.TitanBodyTarget.RightArm");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_TitanBodyTarget_LeftLeg, "Titan.State.TitanBodyTarget.LeftLeg");
UE_DEFINE_GAMEPLAY_TAG(Titan_State_TitanBodyTarget_RightLeg, "Titan.State.TitanBodyTarget.RightLeg");

UE_DEFINE_GAMEPLAY_TAG(Titan_Effect, "Titan.Effect");
UE_DEFINE_GAMEPLAY_TAG(Titan_Effect_AttackCoolDown, "Titan.Effect.AttackCoolDown");
UE_DEFINE_GAMEPLAY_TAG(Titan_Effect_Stamina, "Titan.Effect.Stamina");
UE_DEFINE_GAMEPLAY_TAG(Titan_Effect_HP, "Titan.Effect.HP");
UE_DEFINE_GAMEPLAY_TAG(Titan_Effect_DetectRadius, "Titan.Effect.DetectRadius");
UE_DEFINE_GAMEPLAY_TAG(Titan_Effect_Damage, "Titan.Effect.Damage");
UE_DEFINE_GAMEPLAY_TAG(Titan_Effect_StaminaCost, "Titan.Effect.StaminaCost");

UE_DEFINE_GAMEPLAY_TAG(Player_Effect_Damage, "Player.Effect.Damage");

/*
* Define Event Tags
*/
UE_DEFINE_GAMEPLAY_TAG(Titan_Event, "Titan.Event");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_StartDying, "Titan.Event.StartDying");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_SpawnActor, "Titan.Event.SpawnActor");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_ReleaseActor, "Titan.Event.ReleaseActor");

UE_DEFINE_GAMEPLAY_TAG(Titan_Event_AttackHitCheck, "Titan.Event.AttackHitCheck");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Attack, "Titan.Event.Attack");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Attack_MeleeAttack, "Titan.Event.Attack.MeleeAttack");

UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Attack_SpecialAttack, "Titan.Event.Attack.SpecialAttack");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Attack_SpecialAttack_Skill1, "Titan.Event.Attack.SpecialAttack.Skill1");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Attack_SpecialAttack_Skill2, "Titan.Event.Attack.SpecialAttack.Skill2");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Attack_SpecialAttack_Skill3, "Titan.Event.Attack.SpecialAttack.Skill3");

UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Severed, "Titan.Event.Severed");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Severed_LeftArm, "Titan.Event.Severed.LeftArm");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Severed_RightArm, "Titan.Event.Severed.RightArm");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Severed_LeftLeg, "Titan.Event.Severed.LeftLeg");
UE_DEFINE_GAMEPLAY_TAG(Titan_Event_Severed_RightLeg, "Titan.Event.Severed.RightLeg");

UE_DEFINE_GAMEPLAY_TAG(Player_Data_Damage, "Player.Data.Damage");

UE_DEFINE_GAMEPLAY_TAG(GameplayCue_SeveredBloodSplash, "GameplayCue.SeveredBloodSplash");
UE_DEFINE_GAMEPLAY_TAG(GameplayCue_NormalBloodSplash, "GameplayCue.NormalBloodSplash");

void FAOTGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FAOTGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FAOTGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	/**
	 * GameFrameworkComponentManager init state tags
	 */
	AddTag(Character_State_Attacking, "Character.State.Attacking", "Attacking");
	AddTag(Character_State_Jumping, "Character.State.Jumping", "Jumping");
	AddTag(Character_State_Moving, "Character.State.Moving", "Moving");
	AddTag(Character_State_Rolling, "Character.State.Rolling", "Rolling");
	AddTag(Character_State_Skilling, "Character.State.Skilling", "Skilling");
	AddTag(Character_State_ModeChanging, "Character.State.ModeChanging", "ModeChanging");
	AddTag(Character_State_Stun, "Character.State.Stun", "Stunning");
	AddTag(Titan_AttackDirection_DownToUp, "Titan.AttackDirection.DownToUp", "TitanAttackDownToUp");
	AddTag(Titan_AttackDirection_UpToDown, "Titan.AttackDirection.UpToDown", "TitanAttackUpToDown");
	AddTag(Titan_AttackDirection_LeftToRight, "Titan.AttackDirection.LeftToRight", "TitanAttackLeftToRight");
	AddTag(Titan_AttackDirection_RightToLeft, "Titan.AttackDirection.RightToLeft", "TitanAttackRightToLeft");
	AddTag(Titan_AttackDirection_Straight, "Titan.AttackDirection.Straight", "TitanAttackStraight");
	AddTag(Event_Character_SpawnSpear, "Event.Character.SpawnSpear", "SpawnSpear");
	AddTag(Event_Character_AttackHitCheck, "Event.Character.AttackHitCheck", "AttackHitCheck");
	AddTag(Event_Character_SkillHitCheck, "Event.Character.SkillHitCheck", "SkillHitCheck");
	AddTag(Event_Titan_DamageReceived, "Event.Titan.DamageReceived", "DamageReceived");
}



