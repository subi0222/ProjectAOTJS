#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

/** forward declaration */
class UGameplayTagsManager;

/*
* State Tags
*/
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Attacking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Dead);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Neck_Alive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Neck_Severed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Arm_TwoArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Arm_OneArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Arm_OneArm_LeftArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Arm_OneArm_RightArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Leg_TwoLeg);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Leg_OneLeg);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Leg_OneLeg_LefLeg);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_Leg_OneLeg_RightLeg);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TargetLocation);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TargetLocation_Top);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TargetLocation_Middle);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TargetLocation_Bottom);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TitanBodyTarget);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TitanBodyTarget_LeftArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TitanBodyTarget_RightArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TitanBodyTarget_LeftLeg);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_State_TitanBodyTarget_RightLeg);;

/*
* Event Tags
*/
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_AttackHitCheck);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_StartDying);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_SpawnActor);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_ReleaseActor);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_Neck);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_RightArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_LeftArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_RightLeg);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_LeftLeg);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Attack_MeleeAttack);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Attack_SpecialAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Attack_SpecialAttack_Skill1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Attack_SpecialAttack_Skill2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Attack_SpecialAttack_Skill3);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_LeftArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_RightArm);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_LeftLeg);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Event_Severed_RightLeg);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Effect);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Effect_AttackCoolDown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Effect_Stamina);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Effect_HP);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Effect_DetectRadius);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Effect_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Titan_Effect_StaminaCost);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Effect_Damage);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_SeveredBloodSplash)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_NormalBloodSplash)

/**
 * HakGameplayTags
 * - singleton containing native gameplay tags
 */
struct FAOTGameplayTags
{
	/**
	 * static methods
	 */
	static const FAOTGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	/**
	 * member methods
	 */
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	/**
	 * �Ʒ��� GameplayTag�� �ʱ�ȭ ���� �ܰ踦 �ǹ��Ѵ�:
	 * - GameInstance�� �ʱ�ȭ ������ UGameFrameworkComponentManager�� RegisterInitState�� ��ϵǾ� ����������(linear)�ϰ� ������Ʈ �ȴ�
	 * - �� �ʱ�ȭ GameplayTag�� ������ Actor ���̿� �����Ǹ�, GameFrameworkInitStateInterface ��ӹ��� Ŭ������ �ʱ�ȭ ����(Init State)�� ���¸ӽ�(State Machine)�� ���� ���� ������ �������̽��� �����Ѵ�
	 */
	FGameplayTag Character_State_Moving;
	FGameplayTag Character_State_Jumping;
	FGameplayTag Character_State_Rolling;
	FGameplayTag Character_State_Attacking;
	FGameplayTag Character_State_Skilling;
	FGameplayTag Character_State_ModeChanging;
	FGameplayTag Character_State_Stun;
	FGameplayTag Titan_AttackDirection_DownToUp;
	FGameplayTag Titan_AttackDirection_UpToDown;
	FGameplayTag Titan_AttackDirection_LeftToRight;
	FGameplayTag Titan_AttackDirection_RightToLeft;
	FGameplayTag Titan_AttackDirection_Straight;
	FGameplayTag Event_Character_SpawnSpear;
	FGameplayTag Event_Character_AttackHitCheck;
	FGameplayTag Event_Character_SkillHitCheck;
	FGameplayTag Event_Titan_DamageReceived;

private:
	// static ���� �ʱ�ȭ�� .cpp�� ���ִ� ���� ���� ����!
	static FAOTGameplayTags GameplayTags;
};