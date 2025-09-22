// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Combo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Moba/Utilities/MobaGameplayTags.h"

UGA_Combo::UGA_Combo()
{
	AbilityTags.AddTag(FMobaGameplayTags::Ability_BasicAttack);
	BlockAbilitiesWithTag.AddTag(FMobaGameplayTags::Ability_BasicAttack);
}

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
	// task 播放 技能 动画蒙太奇
	// 允许客户端预测操作 而不是立即执行
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, ComboMontage);

		// 注册 绑定一些 动画的 时间，已到达回调效果
		// 混合淡出， 取消，完成，打断等事件 此处时当这个动画正在执行这些时间是， 则结束 ga
		PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);

		// 必须调用这个函数吗实际去激活蒙太奇 ，否则无任何效果
		PlayMontageAndWait->ReadyForActivation();
	}
}
