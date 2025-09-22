// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Combo.h"

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// 最后两个 布尔函数 ，希望操作被复制,(不用被取消，因为从未执行)
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Casting  GA Combo"));
}
