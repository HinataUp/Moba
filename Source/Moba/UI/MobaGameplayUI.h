// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobaGameplayUI.generated.h"

class UAbilitySystemComponent;
class UMobaValueGauge;
/**
 * 
 */
UCLASS()
class MOBA_API UMobaGameplayUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMobaValueGauge> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMobaValueGauge> ManaBar;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC;
};
