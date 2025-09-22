// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaValueGauge.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMobaValueGauge::NativeConstruct()
{
	Super::NativeConstruct();
	ProgressBar->SetFillColorAndOpacity(BarColor);
}

void UMobaValueGauge::SetValue(float NewValue, float NewMaxValue)
{
	CacheValue = NewValue;
	CacheMaxValue = NewMaxValue;
	
	if (NewMaxValue == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("value not 0: %s"), *GetName());
		return;
	}

	float NewPercent = NewValue / NewMaxValue;
	ProgressBar->SetPercent(NewPercent);

	//设置最大 小数 位数
	const FNumberFormattingOptions FormattingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);

	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(NewValue, &FormattingOptions),
			FText::AsNumber(NewMaxValue, &FormattingOptions)
		)
	);
}

void UMobaValueGauge::ValueChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	SetValue(OnAttributeChangeData.NewValue, CacheMaxValue);
}

void UMobaValueGauge::MaxValueChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	SetValue(CacheValue, OnAttributeChangeData.NewValue);
}

void UMobaValueGauge::SetAndBoundToGameplayAttribute(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute,
                                                     const FGameplayAttribute& MaxAttribute)
{
	if (!ASC)
	{
		return;
	}

	bool bFound;
	float Value = ASC->GetGameplayAttributeValue(Attribute, bFound);
	float MaxValue = ASC->GetGameplayAttributeValue(MaxAttribute, bFound);
	if (bFound)
	{
		SetValue(Value, MaxValue);
	}
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UMobaValueGauge::ValueChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UMobaValueGauge::MaxValueChanged);
}
