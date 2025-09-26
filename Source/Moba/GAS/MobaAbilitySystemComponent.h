// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MobaAbilitySystemComponent.generated.h"


enum class EMobaAbilityInputID : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MOBA_API UMobaAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	UMobaAbilitySystemComponent();
public:
	void ApplyInitialEffects();
	void GiveInitialAbilities();

private:
	// GE 仅作为配置， TSubclassOf指明类型，无需实例
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effect")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	// 角色独特的
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effect")
	TMap<EMobaAbilityInputID, TSubclassOf<UGameplayAbility>> Abilities;

	// 每个人都有的技能
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effect")
	TMap<EMobaAbilityInputID, TSubclassOf<UGameplayAbility>> BasicAbilities;

	// 角色死亡
	void HealthUpdated(const FOnAttributeChangeData& ChangeData);
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> DeathEffect;
};
