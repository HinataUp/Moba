// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Moba/Character/MobaCharacterBase.h"
#include "MobaPlayer.generated.h"

enum class EMobaAbilityInputID : uint8;
class UWidgetComponent;
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
class MOBA_API AMobaPlayer : public AMobaCharacterBase
{
	GENERATED_BODY()

public:
	AMobaPlayer();

	// player 重生后的操作，本质 start 时 也是一次重生，基于次 为玩家激活输入等功能
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TMap<EMobaAbilityInputID, UInputAction*> GameplayAbilityInputActions;

	void HandleLookInput(const FInputActionValue& InputActionValue);
	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleAbilityInput(const FInputActionValue& InputActionValue, EMobaAbilityInputID InputID);

	FVector GetLookRightDir() const;
	FVector GetLookFwdDir() const;
	FVector GetMoveFwdDir() const;

#pragma endregion
};
