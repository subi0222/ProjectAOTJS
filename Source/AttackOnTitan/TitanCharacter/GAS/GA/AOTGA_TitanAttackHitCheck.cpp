// Unreal Engine Marketplace Copyright


#include "TitanCharacter/GAS/GA/AOTGA_TitanAttackHitCheck.h"

#include "AbilitySystemComponent.h"
#include "AttackOnTitan.h"
#include "Character/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "TitanCharacter/AOTTitanBase.h"
#include "TitanCharacter/Components/AOTTitanAttackPartComponent.h"
#include "TitanCharacter/Components/AOTTitanCombatStateComponent.h"
#include "TitanCharacter/GAS/AttributeSet/AOTTitanCombatAttributeSet.h"

void UAOTGA_TitanAttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const UAOTTitanAttackPartComponent* ATP = Cast<UAOTTitanAttackPartComponent>(TriggerEventData->OptionalObject);
	if (!ATP)
	{
		AOT_LOG(LogAOTGAS, Warning, TEXT("%s"),TEXT("Cannot Cast OptionalObject To AttackPartComp"));
		return;
	}

	FVector Start = ATP->GetComponentLocation();
	FVector End = Start + ATP->GetForwardVector() * 200.f;

	float Radius = ATP->GetScaledSphereRadius();
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;

	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());
	
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults,
		Start,
		End,
		ATP->GetComponentQuat(),
		ECC_WorldDynamic,
		CollisionShape,
		Params);

	if (bHit)
	{
		for (FHitResult HitResult : HitResults)
		{
			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(HitResult.GetActor());
			if (BaseCharacter)
			{
				if (!ATP->HasHit(BaseCharacter))
				{
					auto* NonConstATP = const_cast<UAOTTitanAttackPartComponent*>(ATP);
					if (!NonConstATP)
					{
						AOT_LOG(LogAOTGAS, Warning, TEXT("%s"), TEXT("Can Not Cast Non Const"))
						return;
					}
					ApplyDamageEffect(BaseCharacter);
					NonConstATP->RegisterHitCharacter(BaseCharacter);
				}
			}
		}
	}

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugCircle(GetWorld(), Start, Radius, 6, DrawColor);

#endif
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAOTGA_TitanAttackHitCheck::ApplyDamageEffect(ABaseCharacter* HitCharacter)
{
	UAbilitySystemComponent* ASC = HitCharacter->GetAbilitySystemComponent();
	if (ASC && HitEffectClass)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(HitEffectClass, 1.0f, EffectContextHandle);

		if (SpecHandle.IsValid())
		{
			const UAOTTitanCombatAttributeSet* CombatAttributeSet = GetAbilitySystemComponentFromActorInfo()->GetSet<UAOTTitanCombatAttributeSet>();
			if (CombatAttributeSet)
			{
				float Damage = CombatAttributeSet->GetMeleeAttackDamage();
				SpecHandle.Data->SetSetByCallerMagnitude(Player_Effect_Damage, Damage);
			}
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}
