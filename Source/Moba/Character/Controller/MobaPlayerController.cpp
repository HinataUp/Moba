// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Moba/Character/Player/MobaPlayer.h"
#include "Moba/UI/MobaGameplayUI.h"
#include "Net/UnrealNetwork.h"

void AMobaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	MobaPlayer = Cast<AMobaPlayer>(InPawn);
	if (MobaPlayer)
	{
		MobaPlayer->ServerSideInit();
		MobaPlayer->SetGenericTeamId(TeamID);
	}
}

// 客户端应答
void AMobaPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	MobaPlayer = Cast<AMobaPlayer>(P);
	if (MobaPlayer)
	{
		MobaPlayer->ClientSideInit();
		SpawnGameplayUI();
	}
}

void AMobaPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMobaPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId AMobaPlayerController::GetGenericTeamId() const
{
	return TeamID;
}

void AMobaPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMobaPlayerController, TeamID);
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
