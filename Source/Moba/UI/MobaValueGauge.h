// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "MobaValueGauge.generated.h"

struct FGameplayAttribute;
class UAbilitySystemComponent;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class MOBA_API UMobaValueGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetValue(float NewValue, float NewMaxValue);
	void ValueChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void MaxValueChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	void SetAndBoundToGameplayAttribute(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute,
	                                    const FGameplayAttribute& MaxAttribute);

private:
	UPROPERTY(EditAnywhere, Category="Visual")
	FLinearColor BarColor;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UTextBlock* ValueText;

	float CacheValue;
	float CacheMaxValue;
};
