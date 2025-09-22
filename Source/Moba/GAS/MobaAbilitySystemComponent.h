// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MobaAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MOBA_API UMobaAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMobaAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	void ApplyInitialEffects();
private:
	 // GE 仅作为配置， TSubclassOf指明类型，无需实例
	UPROPERTY(EditDefaultsOnly,Category="Gameplay Effect")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	// 角色独特的
	UPROPERTY(EditDefaultsOnly,Category="Gameplay Effect")
	TArray<TSubclassOf<UGameplayAbility>> Abilitys;

	// 每个人都有的
	UPROPERTY(EditDefaultsOnly,Category="Gameplay Effect")
	TArray<TSubclassOf<UGameplayAbility>> BasicAbilitys;
};
