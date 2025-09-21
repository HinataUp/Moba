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
