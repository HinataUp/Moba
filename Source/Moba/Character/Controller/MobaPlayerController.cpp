// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaPlayerController.h"

#include "Moba/Character/Player/MobaPlayer.h"

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
	}
}

void AMobaPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
