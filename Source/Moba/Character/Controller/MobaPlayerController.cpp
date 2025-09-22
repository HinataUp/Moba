// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Moba/Character/Player/MobaPlayer.h"
#include "Moba/UI/MobaGameplayUI.h"

void AMobaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Mobaplayer = Cast<AMobaPlayer>(InPawn);
	if (Mobaplayer)
	{
		Mobaplayer->ServerSideInit();
	}
}

// 客户端应答
void AMobaPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	Mobaplayer = Cast<AMobaPlayer>(P);
	if (Mobaplayer)
	{
		Mobaplayer->ClientSideInit();
		SpawnGameplayUI();
	}
}

void AMobaPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMobaPlayerController::SpawnGameplayUI()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	GameplayUI = CreateWidget<UMobaGameplayUI>(this, GameplayUIClass);
	if (GameplayUI)
	{
		GameplayUI->AddToViewport();
	}
}
