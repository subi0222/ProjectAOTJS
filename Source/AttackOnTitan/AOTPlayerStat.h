
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AOTPlayerStat.generated.h"

USTRUCT(BlueprintType)
struct FAOTPlayerStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAOTPlayerStat() : CurrentHp(0.f),MaxHp(0.0f), CurrentDamage(0.0f), AttackRange(0.0f), AttackRadius(0.0f) , MaxEnergy(0.f),CurrentEnergy(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseMaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseMaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseAttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float BaseAttackRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRadius;	

	FAOTPlayerStat operator+(const FAOTPlayerStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FAOTPlayerStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FAOTPlayerStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}
		return Result;
	}

	FAOTPlayerStat operator-(const FAOTPlayerStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FAOTPlayerStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FAOTPlayerStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] - OtherPtr[i];
		}
		return Result;
	}
};
