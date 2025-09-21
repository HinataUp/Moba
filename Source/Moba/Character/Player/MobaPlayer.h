// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Moba/Character/MobaCharacterBase.h"

#include "MobaPlayer.generated.h"

class UMobaAttributeSet;
class UMobaAbilitySystemComponent;
struct FGameplayEventData;
struct FGameplayTag;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MOBA_API AMobaPlayer : public AMobaCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMobaPlayer();
	// 区分 服务器和 客户端之间所作的事情
	void ServerSideInit();
	void ClientSideInit();
	
	// player 重生后的操作，本质 start 时 也是一次重生，基于次 为玩家激活输入等功能
	virtual void PawnClientRestart() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	TObjectPtr<UCameraComponent> ViewCam;

#pragma region input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	void HandleLookInput(const FInputActionValue& InputActionValue);

	void HandleMoveInput(const FInputActionValue& InputActionValue);
	FVector GetLookRightDir() const;
	FVector GetLookFwdDir() const;
	FVector GetMoveFwdDir() const;

#pragma endregion

#pragma region GAS

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	TObjectPtr<UMobaAbilitySystemComponent> MobaAsc;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	TObjectPtr<UMobaAttributeSet> MobaAs;
#pragma endregion
};
