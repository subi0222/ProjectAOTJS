
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class AttackPointType : uint8
{
	NeckSocket UMETA(DisplayName = NeckSocket),
	LeftShoulderSocket UMETA(DisplayName = LeftShoulderSocket),
	RightShoulderSocket UMETA(DisplayName = RightShoulderSocket),
	LeftElbowSocket UMETA(DisplayName = LeftElbowSocket),
	RightElbowSocket UMETA(DisplayName = RightElbowSocket),
	LeftHandSocket UMETA(DisplayName = LeftHandSocket),
	RightHandSocket UMETA(DisplayName = RightHandSocket),
	END,
};

UENUM(BlueprintType)
enum class StatType : uint8
{
	CurrentHp UMETA(DisplayName = CurrentHp),
	MaxHp UMETA(DisplayName = MaxHp),
	CurrentEnergy UMETA(DisplayName = CurrentEnergy),
	MaxEnergy UMETA(DisplayName = MaxEnergy),
	CurrentDamage UMETA(DisplayName = CurrentDamage),
	AttackRange UMETA(DisplayName = AttackRange),
	AttackRadius UMETA(DisplayName = AttackRadius),
	END,
};

UENUM(BlueprintType)
enum class OpType : uint8
{
	Add UMETA(DisplayName = Add),
	Mul UMETA(DisplayName = Mul),
	Div UMETA(DisplayName = Div),
	Override UMETA(DisplayName = Override),
	End,
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon   UMETA(Displayname = "Weapon"),
	EIT_Potion    UMETA(Displayname = "Potion"),
	EIT_None      UMETA(Displayname = "None")
};

UENUM(BlueprintType)
enum class SpawnType :uint8
{
	SpawnGround,
	CasterAround,
	CasterAttach,
	Target,
};

UENUM(BlueprintType)
enum class SkillActorType : uint8
{
	Projectile,
	Area,
	End,
};

UENUM(BlueprintType)
enum class SkillActorShape : uint8
{
	Box,
	Capsule,
	End,
};