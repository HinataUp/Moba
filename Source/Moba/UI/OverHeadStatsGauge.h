// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadStatsGauge.generated.h"

class UMobaValueGauge;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class MOBA_API UOverHeadStatsGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	void ConfigureWithASC(UAbilitySystemComponent* ASC);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMobaValueGauge> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMobaValueGauge> ManaBar;
};
