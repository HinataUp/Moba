// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaPlayerControllerBase.h"
#include "MobaPlayerController.generated.h"

class AMobaPlayer;
/**
 * 
 */
UCLASS()
class MOBA_API AMobaPlayerController : public AMobaPlayerControllerBase
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* P) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	TObjectPtr<AMobaPlayer> Mobaplayer;
};
