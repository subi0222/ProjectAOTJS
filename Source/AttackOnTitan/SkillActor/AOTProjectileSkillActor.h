// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "SkillActor/AOTSkillActor.h"
#include "AOTProjectileSkillActor.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API AAOTProjectileSkillActor : public AAOTSkillActor
{
	GENERATED_BODY()
public:
	AAOTProjectileSkillActor();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnOverlapBeginProjectile(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
