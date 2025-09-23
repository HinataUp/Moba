// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Combo.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
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

		// 连击触发 等待事件，这里不要写错成end 了 否则 就只能接收到end 事件了
		UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, GetComboChangedEventTag(), nullptr, false
			, false);
		WaitGameplayEvent->EventReceived.AddDynamic(this, &UGA_Combo::ComboChangedEventReceived);
		WaitGameplayEvent->ReadyForActivation();
	}
	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitTargetingEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, GetComboTargetEventTag());
		WaitTargetingEventTask->EventReceived.AddDynamic(this, &UGA_Combo::DoDamage);
		WaitTargetingEventTask->ReadyForActivation();
	}

	SetupWaitComboInputPress();
}

FGameplayTag UGA_Combo::GetComboChangedEventTag()
{
	return FMobaGameplayTags::Ability_Combo_Change;
}

FGameplayTag UGA_Combo::GetComboChangedEventEndTag()
{
	return FMobaGameplayTags::Ability_Combo_Change_End;
}

FGameplayTag UGA_Combo::GetComboTargetEventTag()
{
	return FMobaGameplayTags::Ability_Combo_Damage;
}

void UGA_Combo::SetupWaitComboInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &UGA_Combo::HandleInputPress);
	WaitInputPress->ReadyForActivation();
}

void UGA_Combo::HandleInputPress(float TimeWaited)
{
	SetupWaitComboInputPress();
	TryCommitCombo();
}

void UGA_Combo::TryCommitCombo()
{
	if (NextComboName == NAME_None)
	{
		return;
	}

	UAnimInstance* OwnerAnimInst = GetOwnerAnimInstance();
	if (!OwnerAnimInst)
	{
		return;
	}
	OwnerAnimInst->Montage_SetNextSection(OwnerAnimInst->Montage_GetCurrentSection(ComboMontage),
	                                      NextComboName, ComboMontage);
}

void UGA_Combo::DoDamage(FGameplayEventData Data)
{
	TArray<FHitResult> HitResults = GetHitResultFromSweepLocationTargetData(Data.TargetData,
	                                                                        30.f,
	                                                                        true,
	                                                                        true);

	for (const FHitResult& HitResult : HitResults)
	{
		TSubclassOf<UGameplayEffect> GameplayEffect = GetDamageEffectForCurrentCombo();
		FGameplayEffectSpecHandle EfffectSpecHandle = MakeOutgoingGameplayEffectSpec(
			GameplayEffect, GetAbilityLevel(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));

		ApplyGameplayEffectSpecToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo,
		                                EfffectSpecHandle,
		                                UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(
			                                HitResult.GetActor()));
	}
}

TSubclassOf<UGameplayEffect> UGA_Combo::GetDamageEffectForCurrentCombo() const
{
	if (UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance())
	{
		FName CurrentSectionName = OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage);
		if (const TSubclassOf<UGameplayEffect>* FoundEffectPtr = DamageEffectMap.Find(CurrentSectionName))
		{
			return *FoundEffectPtr;
		}
	}

	return DefaultDamageEffect;
}

// 一个是播放 蒙太奇到指定帧 通知 可以连击， 一个时 end tag标志 不允许连击。
// 只允许 通知下一个连击 tag  到 end 之间 内可以触发连击
void UGA_Combo::ComboChangedEventReceived(FGameplayEventData Data)
{
	FGameplayTag EventTag = Data.EventTag;
	// UE_LOG(LogTemp, Warning, TEXT("EventTag  : %s"), *EventTag.ToString());
	// 这里是相应end tag
	if (EventTag == GetComboChangedEventEndTag())
	{
		NextComboName = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT("Next combo is cleared"));
		return;
	}
	TArray<FName> TagNames;
	// 仅仅是把标签分割 然后获取最后一个tag 分割eventtag 第二个是存入的数组
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();
	UE_LOG(LogTemp, Warning, TEXT("next combo is now: %s"), *NextComboName.ToString());
}
