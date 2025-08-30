// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AOTTitanStatData.generated.h"

/**
 * 
 */

UCLASS()
class ATTACKONTITAN_API UAOTTitanStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	FORCEINLINE float GetHP() const { return HP; }	

	FORCEINLINE float GetDetectRadius() const { return DetectRadius; }

	FORCEINLINE float GetStamina() const { return Stamina; }

	FORCEINLINE float GetMeleeAttackDamage() const {return MeleeAttackDamage;}
	
	FORCEINLINE float GetSpecialAttackDamage() const {return SpecialAttackDamage;}


private:

	UPROPERTY(EditDefaultsOnly)
	float HP;

	UPROPERTY(EditDefaultsOnly)
	float Stamina;

	UPROPERTY(EditDefaultsOnly)
	float DetectRadius;

	UPROPERTY(EditDefaultsOnly)
	float MeleeAttackDamage;

	UPROPERTY(EditDefaultsOnly)
	float SpecialAttackDamage;

};
