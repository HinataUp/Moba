// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaGameplayUI.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "MobaValueGauge.h"
#include "Moba/GAS/AttributeSets/MobaAttributeSet.h"

void UMobaGameplayUI::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwnerASC)
	{
		HealthBar->SetAndBoundToGameplayAttribute(OwnerASC, UMobaAttributeSet::GetHealthAttribute(),
		                                          UMobaAttributeSet::GetMaxHealthAttribute());

		ManaBar->SetAndBoundToGameplayAttribute(OwnerASC, UMobaAttributeSet::GetManaAttribute(),
		                                        UMobaAttributeSet::GetMaxManaAttribute());
	}
}
