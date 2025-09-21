// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaAbilitySystemComponent.h"


// Sets default values for this component's properties
UMobaAbilitySystemComponent::UMobaAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMobaAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UMobaAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMobaAbilitySystemComponent::ApplyInitialEffects()
{

	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}
	for (const TSubclassOf<UGameplayEffect> EffectClass : InitialEffects)
	{
		// ApplyGameplayEffectToSelf() 需要创建实力，且手动管理生命周期
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}
