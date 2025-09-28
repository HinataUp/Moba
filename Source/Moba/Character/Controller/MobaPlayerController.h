// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "MobaPlayerControllerBase.h"
#include "MobaPlayerController.generated.h"

class UMobaGameplayUI;
class AMobaPlayer;
/**
 * 
 */
UCLASS()
class MOBA_API AMobaPlayerController : public AMobaPlayerControllerBase, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* P) override;
	virtual void OnUnPossess() override;
	
	//将团队代理分配给给定的 TeamID
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	//将团队代理分配给给定的 TeamID
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void SpawnGameplayUI();

	UPROPERTY()
	TObjectPtr<AMobaPlayer> MobaPlayer;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UMobaGameplayUI> GameplayUIClass;

	UPROPERTY()
	TObjectPtr<UMobaGameplayUI> GameplayUI;

	UPROPERTY(Replicated)
	FGenericTeamId TeamID;
};
