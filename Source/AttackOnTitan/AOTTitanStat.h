// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "AOTTitanStat.generated.h"

UENUM(BlueprintType)
enum class ETitanType : uint8
{
	NormalTitan,
	AttackTitan,
	BeastTitan,
	ArmoredTitan,
	ColossalTitan,
	MAX UMETA(Hidden)
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FAOTTitanStat : public FTableRowBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETitanType TitanName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	float HP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	float Stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	float DetectRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	float SpecialAttackRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stat")
	float SpecialAttackDamage;


};
