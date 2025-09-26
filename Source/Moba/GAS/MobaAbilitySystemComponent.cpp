// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaAbilitySystemComponent.h"

#include "AttributeSets/MobaAttributeSet.h"

UMobaAbilitySystemComponent::UMobaAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UMobaAttributeSet::GetHealthAttribute()).AddUObject(
		this, &UMobaAbilitySystemComponent::HealthUpdated);
}

void UMobaAbilitySystemComponent::ApplyInitialEffects()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}
	for (const TSubclassOf<UGameplayEffect>& EffectClass : InitialEffects)
	{
		// ApplyGameplayEffectToSelf() 需要创建实力，且手动管理生命周期
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UMobaAbilitySystemComponent::GiveInitialAbilities()
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}
	for (const TPair<EMobaAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : Abilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 0, static_cast<int32>(AbilityPair.Key), nullptr));
	}
	for (const TPair<EMobaAbilityInputID, TSubclassOf<UGameplayAbility>>& AbilityPair : BasicAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 1, static_cast<int32>(AbilityPair.Key), nullptr));
	}
}

// 服务器 才有权决定生死
void UMobaAbilitySystemComponent::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (!GetOwner())return;
	if (ChangeData.NewValue <= 0 && GetOwner()->HasAuthority() && DeathEffect)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = MakeOutgoingSpec(DeathEffect, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
	}
}
