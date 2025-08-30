// Unreal Engine Marketplace Copyright

#pragma once

#include "CoreMinimal.h"
#include "Data/AOTSkillActorData.h"
#include "AOTProjectileSkillActorData.generated.h"

/**
 * 
 */
UCLASS()
class ATTACKONTITAN_API UAOTProjectileSkillActorData : public UAOTSkillActorData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Projectile)
	float Speed;
	// 캐릭터의 앞방향에서 시계방향으로 얼마나 회전시킨방향으로 날릴건지 결정.
	UPROPERTY(EditAnywhere, Category = Projectile)
	float DirOffset;
	// xy평면에 평행하게 발사할건지 위에서 아래로 혹은 아래에서 위로발사할건지
	UPROPERTY(EditAnywhere, Category = Projectile)
	uint8 bDirFront :1;
	UPROPERTY(EditAnywhere, Category = Projectile)
	uint8 bGravity : 1;
	// Target을 맞으면 Actor가 Destroy된다. 이 값이 false면 타겟을 관통하고 간다.
	UPROPERTY(EditAnywhere, Category = Projectile)
	uint8 bTarget : 1;
};
