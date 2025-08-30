// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "TitanAttackData.generated.h"

UENUM(BlueprintType)  // 블루프린트에서 사용 가능하도록 설정
enum class EAttackType : uint8
{
	RightHand UMETA(DisplayName = "Right Hand Attack"),
	LeftHand UMETA(DisplayName = "Left Hand Attack"),
	TwoHanded UMETA(DisplayName = "Two Handed Attack"),
	RightFoot UMETA(DisplayName = "Left Foot Attack"),
	LeftFoot UMETA(DisplayName = "Right Foot Attack")
};

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float AttackRadius;

	UPROPERTY(EditDefaultsOnly)
	EAttackType AttackType;

	UPROPERTY(EditDefaultsOnly)
	FString SectionName;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

};


/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UTitanAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	TArray<FAttackData> GetAttackDataArray() const;
	int8 GetSequenceNum() const;
	FAttackData GetAttackData(int32 Index) const;

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int8 SequenceNum;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FAttackData> AttackDataArray;
	
};
