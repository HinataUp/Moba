// Fill out your copyright notice in the Description page of Project Settings.


#include "OverHeadStatsGauge.h"

#include "MobaValueGauge.h"
#include "Moba/GAS/AttributeSets/MobaAttributeSet.h"

void UOverHeadStatsGauge::ConfigureWithASC(UAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		HealthBar->SetAndBoundToGameplayAttribute(ASC, UMobaAttributeSet::GetHealthAttribute(),
		                                          UMobaAttributeSet::GetMaxHealthAttribute());

		ManaBar->SetAndBoundToGameplayAttribute(ASC, UMobaAttributeSet::GetManaAttribute(),
		                                        UMobaAttributeSet::GetMaxManaAttribute());
	}
}
