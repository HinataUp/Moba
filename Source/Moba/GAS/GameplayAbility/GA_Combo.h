// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaGameplayAbilityBase.h"
#include "GA_Combo.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_API UGA_Combo : public UMobaGameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData);
};
