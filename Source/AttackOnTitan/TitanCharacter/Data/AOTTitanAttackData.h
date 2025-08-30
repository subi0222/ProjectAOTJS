// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AOTTitanAttackData.generated.h"

UENUM(BlueprintType)
enum class EAttackMode : uint8
{
	LightAttack UMETA(DisplayName = "LightAttack"),
	HardAttack UMETA(DisplayName = "HardAttack")
};

UENUM(BlueprintType)
enum class ESpawnLocationType : uint8
{
	Target UMETA(DisplayName = "Below"),
	Owner UMETA(DisplayName = "Owner")
};

USTRUCT()
struct FSpawnActorInformation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int SpawnActorNum;

	UPROPERTY(EditDefaultsOnly)
	float SpawnActorScale;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	uint8 bNeedToRandomSpawn : 1;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnActorClass;

	UPROPERTY(EditDefaultsOnly)
	ESpawnLocationType SpawnLocationType;

	UPROPERTY(EditDefaultsOnly)
	FName SpawnSocketName;
	
	bool operator==(const FSpawnActorInformation& Other) const
	{
		return SpawnActorNum == Other.SpawnActorNum
			&& SpawnActorClass == Other.SpawnActorClass
			&& SpawnLocationType == Other.SpawnLocationType
			&& SpawnSocketName == Other.SpawnSocketName;
	}
};

USTRUCT()
struct FComboAttackInformation
{
	GENERATED_BODY()

	//공격 Montage에 있는 AnimState애서 처리
	UPROPERTY(EditdefaultsOnly, Category = Motion)
	uint8 bMotionWarp : 1;

	//공격 Montage에 있는 AnimState애서 처리
	UPROPERTY(EditDefaultsOnly, Category = Motion)
	uint8 bRotating : 1;

	//공격 Montage에 있는 AnimNotify를 통해 확인
	UPROPERTY(EditDefaultsOnly, Category = Check)
	float AttackAllowedDistance;

	UPROPERTY(EditDefaultsOnly, Category = Check)
	float AttackAllowedThresholdDistance;

	//각 Titan마다 기본 공격 데미지가 있고, 공격 모드에 따라 정확한 데미지 값은 배가 됨.
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	EAttackMode AttackMode;

	//콤보 공격시 Titan상태가 공격에 맞지 않는 경우를 확인
	UPROPERTY(EditDefaultsOnly, Category = Tag)
	FGameplayTagContainer NotAcceptedTitanStateTags;

	//해당 공격을 할 때에 Stamina가 얼마나 다는지를 확인
	UPROPERTY(EditDefaultsOnly, Category = Cost)
	float AttackStaminaCost = -1.0f;

	//몽타주 섹션 네임
	UPROPERTY(EditDefaultsOnly, Category = Section)
	FString MontageSectionName;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnActorInformation> SpawnActorInformations;

	bool operator==(const FComboAttackInformation& Other) const
	{
		return bMotionWarp == Other.bMotionWarp
			&& bRotating == Other.bRotating
			&& FMath::IsNearlyEqual(AttackAllowedDistance, Other.AttackAllowedDistance)
			&& AttackMode == Other.AttackMode
			&& NotAcceptedTitanStateTags == Other.NotAcceptedTitanStateTags
			&& FMath::IsNearlyEqual(AttackStaminaCost, Other.AttackStaminaCost)
			&& MontageSectionName == Other.MontageSectionName
			&& SpawnActorInformations == Other.SpawnActorInformations;
	}

	bool IsValid() const
	{
		return AttackStaminaCost != -1.0f;
	}
	
};


USTRUCT()
struct FNextAttackInformation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FComboAttackInformation> ComboAttackInformations;
};

USTRUCT()
struct FAttackInformation
{
	GENERATED_BODY()

	//공격을 하는 동안 붙어 있을 태그.
	UPROPERTY(EditDefaultsOnly, Category = Tag)
	FGameplayTag AttackTag;

	//타겟이 어디에 있는지에 따라 공격이 다르게 하기 위함.
	UPROPERTY(EditDefaultsOnly, Category = Tag)
	FGameplayTagContainer TargetLocationTags;

	//공격 시작 시 Titan상태가 공격에 맞지 않는 경우를 확인
	UPROPERTY(EditDefaultsOnly, Category = Tag)
	FGameplayTagContainer NotAcceptedTitanStateTags;

	//공격 Montage
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	TObjectPtr<UAnimMontage> AttackMontage;

	//공격 Montage에 있는 AnimState에서 처리
	UPROPERTY(EditdefaultsOnly, Category = Motion)
	uint8 bMotionWarp : 1;

	UPROPERTY(EditdefaultsOnly, Category = Motion)
	uint8 bRotating : 1;

	UPROPERTY(EditdefaultsOnly, Category = Check)
	float AttackAllowedDistance;

	UPROPERTY(EditdefaultsOnly, Category = Check)
	float AttackAllowedThresholdDistance;

	//각 Titan마다 기본 공격 데미지가 있고, 공격 모드에 따라 정확한 데미지 값은 배가 됨.
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	EAttackMode AttackMode;

	//콤보 공격일 경우 해당 공격을 진행할 수 있는지 확인하는 NextAttackInformation배열을 갖고 있음
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TArray<FNextAttackInformation> NextAttackInformations;

	//해당 공격을 할 때에 Stamina가 얼마나 다는지를 확인
	UPROPERTY(EditDefaultsOnly, Category = Cost)
	float AttackStaminaCost = -1.0f;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnActorInformation> SpawnActorInformations;

	bool IsValid() const
	{
		return !(AttackStaminaCost == -1.0f);
	}

};


/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTTitanAttackData : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE TArray<FAttackInformation> GetAttackInformations() const { return Attacks; }

private:
	UPROPERTY(EditDefaultsOnly, Category = AttackData)
	TArray<FAttackInformation> Attacks;
};
