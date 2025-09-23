// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
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

	virtual void BeginPlay() override;

	bool IsLocalControlledByPlayer() const;

	virtual void PossessedBy(AController* NewController) override;

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

#pragma region GAS

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	TObjectPtr<UMobaAbilitySystemComponent> MobaAsc;

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay Ability")
	TObjectPtr<UMobaAttributeSet> MobaAs;
#pragma endregion

	// UI
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverheadUIComponent;

	void ConfigureOverheadWidget();

	// 计时器 辅助 取消tick ,用作检查是否显示敌人血量
	UPROPERTY(EditDefaultsOnly, Category="UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;

	// 距离平方 避开平方根 计算
	UPROPERTY(EditDefaultsOnly, Category="UI")
	float HeadStatGaugeVisibilityRangeSquared = 1000000.f;

	FTimerHandle HeadStartGaugeVisibilityUpdateTimerHandle;
	void UpdateHeadGaugeVisibility();
};
