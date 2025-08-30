// Unreal Engine Marketplace Copyright


#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttackOnTitan/Data/AOTBaseComboAttackData.h"
#include "AttackOnTitan/Player/AOTPlayerController.h"
#include "Engine/DamageEvents.h"
#include "AttackOnTitan/Player/AOTWeapon_Spear.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AOTEnemyDetectComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/Interaction/InteractionComponent.h"
#include "Components/Inventory/InventoryComponent.h"
#include "Components/Interaction/InteractionWidget.h"
#include "Item/BaseInteractionItem.h"
#include "UI/AOTPlayerHUD.h"
#include "Player/AOTPlayerState.h"
#include "GAS/AOTPlayerAbilitySystemComponent.h"
#include "Data/AOTSkillData.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = nullptr;
	UE_LOG(LogTemp, Error, TEXT("ABaseCharacter::ABaseCharacter : %lld"), this);

	DetectEnemyComponent = CreateDefaultSubobject<UAOTEnemyDetectComponent>(TEXT("EnemyDetectComponent"));
	DetectEnemyComponent->OnChangeActor.BindUObject(this, &ABaseCharacter::UpdateTargetActorCallback);
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	GetMesh()->SetCollisionProfileName("NoCollision");

	LeftWeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftWeaponMesh"));	
	LeftWeaponMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftHandSocket"));

	RightWeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightWeaponMesh"));	
	RightWeaponMeshComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandSocket"));

	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();	
	
	LeftWeaponMeshComp->SetSkeletalMesh(LeftWeaponMesh);
	RightWeaponMeshComp->SetSkeletalMesh(RightWeaponMesh);
	
	/*Interaction*/
	if (APlayerController* AOTPlayerController = Cast<APlayerController>(GetController()))
	{
		if (AAOTPlayerHUD* AOTPlayerHUD = Cast<AAOTPlayerHUD>(AOTPlayerController->GetHUD()))
		{
			InteractionComponent->SetInteractWidget(AOTPlayerHUD->GetInteractionWidget());
			InteractionComponent->SetWidgetVisibiity(ESlateVisibility::Hidden);
		}
	}
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAOTPlayerState* PS = Cast<AAOTPlayerState>(GetPlayerState());
	if (PS)
	{
		ASC = Cast<UAOTPlayerAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		ASC->InitAbilityActorInfo(PS, this);

		for (auto GA : StartAbilities)
		{
			FGameplayAbilitySpec Spec(GA);
			ASC->GiveAbility(Spec);
		}

		for (auto GA : StartInputAbilities)
		{
			FGameplayAbilitySpec Spec(GA.Value);
			Spec.InputID = GA.Key;
			ASC->GiveAbility(Spec);
		}

		for (auto SkillData : SkillDatas)
		 	ASC->SetSkillData(SkillData->SkillNum, SkillData);

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ModeType == Mode::LockIn)
	{
		FVector TargetLocation = CurrentLockInActor->GetActorLocation();
		FVector CurrentLocation = GetActorLocation();
		FVector Dir = (TargetLocation - CurrentLocation).GetSafeNormal();
		float Speed = 1500.f;
		float Distance = FVector::Dist(TargetLocation, CurrentLocation);
		if (Distance < 50.f)
		{
			ModeType = Mode::DualBlade;
			APlayerController* PC = CastChecked<APlayerController>(GetController());
			UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			Subsystem->AddMappingContext(IC_DualBladeMode, 0);
			Subsystem->RemoveMappingContext(IC_LockInMode);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			return;
		}
		FVector NewLocation = CurrentLocation + Dir * DeltaTime * Speed;
		SetActorLocation(NewLocation, true);
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = CastChecked<APlayerController>(GetController());
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IC_DualBladeMode, 0);
		}
	}

	UEnhancedInputComponent* PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		
		// PlayerInput->BindAction(IA_MoveToAttackPoint, ETriggerEvent::Started, this, &ABaseCharacter::OnMoveToATKPoint);
		 PlayerInput->BindAction(IA_DetectEnemy, ETriggerEvent::Started, this, &ABaseCharacter::OnDetectEnemy);
		 PlayerInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &ABaseCharacter::Interact);
		 PlayerInput->BindAction(IA_NextDetectEnemy, ETriggerEvent::Started, this, &ABaseCharacter::OnNextDetectEnemy);
		 PlayerInput->BindAction(IA_TitanMode, ETriggerEvent::Started, this, &ABaseCharacter::OnTitanMode);
		// PlayerInput->BindAction(IA_Rolling, ETriggerEvent::Triggered, this, &ABaseCharacter::InputPressed, 100);
		// PlayerInput->BindAction(IA_ChangeMode, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 101);
		// PlayerInput->BindAction(IA_BaseAttack, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed,102);		
		// PlayerInput->BindAction(IA_SpearAttack, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed,103);
		// PlayerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 104);
		// PlayerInput->BindAction(IA_Skill01, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 105);
		// PlayerInput->BindAction(IA_Skill02, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 106);
		// PlayerInput->BindAction(IA_Skill03, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 107);
		// PlayerInput->BindAction(IA_Skill04, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 108);
		//PlayerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed,100);
		//PlayerInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABaseCharacter::InputReleased, 100);
		PlayerInput->BindAction(IA_Dash, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 101);
		PlayerInput->BindAction(IA_ChangeMode, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 102);
		PlayerInput->BindAction(IA_Rolling, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 103);
		PlayerInput->BindAction(IA_BaseAttack, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 200);
		PlayerInput->BindAction(IA_SpearAttack, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 201);
		PlayerInput->BindAction(IA_Skill01, ETriggerEvent::Started, this, &ABaseCharacter::InputPressed, 202);
	}
}


void ABaseCharacter::ChangeMode()
{
	APlayerController* PC = CastChecked<APlayerController>(GetController());
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (Subsystem)
		{
			if (ModeType == Mode::DualBlade)
			{
				ModeType = Mode::Spear;
				Subsystem->RemoveMappingContext(IC_DualBladeMode);
				Subsystem->AddMappingContext(IC_SpearMode, 0);

				FTransform RightTransform = FTransform::Identity;
				RightTransform.SetScale3D(FVector(0.5f, 0.5f, 0.7f));
				RightSpear = GetWorld()->SpawnActorDeferred<AAOTWeapon_Spear>(SpearClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				RightSpear->SetInitialSpeed(0.f);
				RightSpear->SetVelocity(FVector(0.f, 0.f, 0.f));
				RightSpear->SetActorEnableCollision(false);
				RightSpear->FinishSpawning(RightTransform);
				RightSpear->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandSocket"));
				UPrimitiveComponent* PrimitiveComponent = RightSpear->FindComponentByClass<UPrimitiveComponent>();
				if (PrimitiveComponent)
					PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

				FTransform LeftTransform = FTransform::Identity;
				LeftTransform.SetScale3D(FVector(0.5f, 0.5, 0.7f));
				LeftSpear = GetWorld()->SpawnActorDeferred<AAOTWeapon_Spear>(SpearClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				LeftSpear->SetInitialSpeed(0.f);
				LeftSpear->SetVelocity(FVector(0.f, 0.f, 0.f));
				LeftSpear->SetActorEnableCollision(false);
				LeftSpear->FinishSpawning(LeftTransform);
				LeftSpear->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandSocket"));
				PrimitiveComponent = LeftSpear->FindComponentByClass<UPrimitiveComponent>();
				if (PrimitiveComponent)
					PrimitiveComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
			}
			else
			{
				ModeType = Mode::DualBlade;
				Subsystem->RemoveMappingContext(IC_SpearMode);
				Subsystem->AddMappingContext(IC_DualBladeMode, 0);
				RightWeaponMeshComp->SetSkeletalMesh(RightWeaponMesh);
				LeftWeaponMeshComp->SetSkeletalMesh(LeftWeaponMesh);
			}			
		}		
	}
}

AHUD* ABaseCharacter::GetHUD()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		return PC->GetHUD();
	}
	return nullptr;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	
	return 0;
}


// **************************************************************************************************************
// ******************************************** Attack **********************************************************


void ABaseCharacter::InputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void ABaseCharacter::InputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void ABaseCharacter::UpdateTargetActorCallback(AActor* NewActor)
{
	if (NewActor == nullptr)
		MotionWarpingComponent->RemoveWarpTarget(FName("TargetActor"));
	else
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromComponent(FName("TargetActor"), NewActor->GetRootComponent(), FName(NAME_None), true);
	}
}

void ABaseCharacter::Interact()
{
	// Check Character Condition
	if (InteractionComponent->GetDetectedActor() == nullptr) return;

	// Cast Interaction Interface and Run Interaction
	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(InteractionComponent->GetDetectedActor());
	if (InteractionInterface)
	{
		InteractionInterface->Interact(InteractionComponent);
	}
}

void ABaseCharacter::Pickup(ABaseInteractionItem* newItem)
{
	if (newItem)
	{
		FPickupItemData* ItemData = newItem->GetItemData();
		if (ItemData)
		{
			if (InventoryComponent->AddItemSlot(*ItemData, newItem->GetItemQuantity()))
			{
				newItem->Destroy();
			}
		}
	}
}

void ABaseCharacter::UseItem(int32 index)
{
	if (InventoryComponent)
	{
		InventoryComponent->UseItemSlot(index);
	}
}



void ABaseCharacter::RotateToTarget()
{
	AActor* TargetActor = DetectEnemyComponent->GetCurrentActor();
	if (TargetActor)
	{
		FVector LocationDiff = TargetActor->GetActorLocation() - GetActorLocation();
		FRotator NewRotation = LocationDiff.Rotation();
		SetActorRotation(NewRotation);
	}
}

// *************************************** Rolling ********************************************

void ABaseCharacter::OnMoveToATKPoint()
{
	APlayerController* PC = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	Subsystem->AddMappingContext(IC_LockInMode, 0);
	Subsystem->RemoveMappingContext(IC_DualBladeMode);
	CurrentLockInActor = DetectEnemyComponent->GetCurrentActor();
	if (CurrentLockInActor == nullptr)
		return;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	ModeType = Mode::LockIn;
}

void ABaseCharacter::OnDetectEnemy()
{
	DetectEnemyComponent->DetectEnemy(2000,2000);
}

void ABaseCharacter::OnNextDetectEnemy()
{
	DetectEnemyComponent->NextEnemy();
}

void ABaseCharacter::OnTitanMode()
{
	if (ModeType == Mode::DualBlade || ModeType == Mode::Spear)
	{
		APlayerController* PC = CastChecked<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		Subsystem->AddMappingContext(IC_TitanMode, 0);
		if (ModeType == Mode::DualBlade)
		{
			Subsystem->RemoveMappingContext(IC_DualBladeMode);
			RightWeaponMeshComp->SetSkeletalMesh(nullptr);
			LeftWeaponMeshComp->SetSkeletalMesh(nullptr);
		}
		else if (ModeType == Mode::Spear)
		{
			Subsystem->RemoveMappingContext(IC_SpearMode);
			RightSpear->SetLifeSpan(0.01f);
			LeftSpear->SetLifeSpan(0.01f);
		}
		SetActorScale3D(FVector(5.f, 5.f, 5.f));
		ModeType = Mode::Titan;
	}
	else if (ModeType == Mode::Titan)
	{
		APlayerController* PC = CastChecked<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		Subsystem->AddMappingContext(IC_DualBladeMode, 0);
		Subsystem->RemoveMappingContext(IC_TitanMode);
		RightWeaponMeshComp->SetSkeletalMesh(RightWeaponMesh);
		LeftWeaponMeshComp->SetSkeletalMesh(LeftWeaponMesh);
		SetActorScale3D(FVector(1.f, 1.f, 1.f));
		ModeType = Mode::DualBlade;
	}
}

bool ABaseCharacter::IsDead()
{
	return false;
}