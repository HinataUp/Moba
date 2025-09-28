// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaAI.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


// Sets default values
AMobaAI::AMobaAI()
{
	PrimaryActorTick.bCanEverTick = true;
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->PeripheralVisionAngleDegrees = 180.f;

	AIPerceptionComp->ConfigureSense(*SightConfig);
}

void AMobaAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetGenericTeamId(FGenericTeamId(0));

	if (IGenericTeamAgentInterface* PawnTeamInterface = Cast<IGenericTeamAgentInterface>(InPawn))
	{
		PawnTeamInterface->SetGenericTeamId(GetGenericTeamId());
	}
}
