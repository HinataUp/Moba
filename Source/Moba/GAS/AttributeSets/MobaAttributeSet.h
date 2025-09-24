// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "MobaAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 *  需要注意 添加任何的 属性集 都需要重新编译，而不是 live coding（会crash）
 */
UCLASS()
class MOBA_API UMobaAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, Health)
	// ATTRIBUTE_ACCESSORS(UMobaAttributeSet, CachedHealthPercent)
	// ATTRIBUTE_ACCESSORS(UMobaAttributeSet, CachedManaPercent)
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MaxMana)
	// ATTRIBUTE_ACCESSORS(UMobaAttributeSet, AttackDamage)
	// ATTRIBUTE_ACCESSORS(UMobaAttributeSet, Armor)
	// ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MoveSpeed)
	// ATTRIBUTE_ACCESSORS(UMobaAttributeSet, MoveAcceleration)

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 在对属性进行任何修改之前调用。这比 PreAttributeModifyPostAttribute 修改的级别低。
	// 这里没有提供额外的上下文，因为任何东西都可以触发它。执行的效果、基于持续时间的效果、移除的效果、应用免疫、更改叠加规则等。
	// 这个函数旨在强制执行诸如“Health = Clamp（Health， 0， MaxHealth）”之类的内容
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 在执行 GameplayEffect 以修改属性的基本值后立即调用。无法再进行更改。请注意，这仅在“执行”期间调用。
	// 例如，对属性的“基本值”的修改。在应用 GameplayEffect 期间不会调用它，例如 5 秒 +10 移动速度增益。
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

private:
	/*
	 * FGameplayAttributeData 一个结构体，包含 base  和cur 两个value
	 * base： 最初设置的 基础值，（没有其他的额外条件时 本身的数值）
	 * cur 的值是基于 base 和一些额外的计算 显示的基础的数值， （cur 值会因为buff 装备这些变化， 但是base 不会变）
	 * 所以 其实如果定义 min 和max health 都是用base  定义了两个，  而不是 拿 cur 当作现在的值 或者 max值
	 */
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;

	// UPROPERTY()
	// FGameplayAttributeData AttackDamage;
	//
	// UPROPERTY()
	// FGameplayAttributeData Armor;
	//
	// UPROPERTY()
	// FGameplayAttributeData MoveSpeed;
	//
	// UPROPERTY()
	// FGameplayAttributeData MoveAcceleration;
	//
	// UPROPERTY()
	// FGameplayAttributeData CachedHealthPercent;
	//
	// UPROPERTY()
	// FGameplayAttributeData CachedManaPercent;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	// UFUNCTION()
	// void OnRep_AttackDamage(const FGameplayAttributeData& OldValue);
	//
	// UFUNCTION()
	// void OnRep_Armor(const FGameplayAttributeData& OldValue);
	//
	// UFUNCTION()
	// void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	//
	// UFUNCTION()
	// void OnRep_MoveAcceleration(const FGameplayAttributeData& OldValue);
};
