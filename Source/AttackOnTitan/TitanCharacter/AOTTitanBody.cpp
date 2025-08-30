// Unreal Engine Marketplace Copyright


#include "TitanCharacter/AOTTitanBody.h"

#include "Components/BoxComponent.h"

// Sets default values
AAOTTitanBody::AAOTTitanBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	RootComponent = BoxComp;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("SkeletalMesh");
	StaticMeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAOTTitanBody::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
}

// Called every frame
void AAOTTitanBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

