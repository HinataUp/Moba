// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Abilities/GameplayAbility.h"
#include "MobaGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class MOBA_API UMobaGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UAnimInstance* GetOwnerAnimInstance() const;

	// 公用的sweep检测函数
	TArray<FHitResult> GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
	                                                           float SphereSweepRadius = 30.f,
	                                                           ETeamAttitude::Type TargetTeam = ETeamAttitude::Hostile,
	                                                           bool bDrawDebug = false,
	                                                           bool bIgnoreSelf = true) const;
};
