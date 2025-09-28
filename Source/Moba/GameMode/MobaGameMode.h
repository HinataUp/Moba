// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaGameModeBase.h"
#include "GenericTeamAgentInterface.h"
#include "MobaGameMode.generated.h"


/**
 * 
 */
UCLASS()
class MOBA_API AMobaGameMode : public AMobaGameModeBase
{
	GENERATED_BODY()
	AMobaGameMode();

public:
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;

private:
	static FGenericTeamId GetTeamIDForPlayer(const APlayerController* PlayerController);

	AActor* FindNextStartSpotForTeam(const FGenericTeamId& TeamID) const;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TMap<FGenericTeamId, FName> TeamStartSpotTagMap;
	
};
