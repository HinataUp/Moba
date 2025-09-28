// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaAIControllerBase.h"
#include "MobaAI.generated.h"

UCLASS()
class MOBA_API AMobaAI : public AMobaAIControllerBase
{
	GENERATED_BODY()

public:
	AMobaAI();

	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAISenseConfig_Sight* SightConfig;
};
