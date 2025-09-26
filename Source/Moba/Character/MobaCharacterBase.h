// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "MobaCharacterBase.generated.h"

struct FGameplayTag;
class UWidgetComponent;
class UMobaAttributeSet;
class UMobaAbilitySystemComponent;

UCLASS()
class MOBA_API AMobaCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMobaCharacterBase();
	// 区分 服务器和 客户端之间所作的事情
	void ServerSideInit();
	void ClientSideInit();

	// player 重生后的操作，本质 start 时 也是一次重生，基于次 为玩家激活输入等功能
	virtual void PawnClientRestart() override;

	bool IsLocalControlledByPlayer() const;

	virtual void PossessedBy(AController* NewController) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	void UpdateHeadGaugeVisibility() const;

	//  Death and Respawn 
private:
	void SetStatusGaugeEnabled(bool bIsEnabled);
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathMontage;

	void PlayDeathAnimation();
	void BindGASChangeDelegates();
	void DeathTagUpdated(const FGameplayTag Tag, int32 NewCount);
	// Death and Respawn
	void StartDeathSequence();
	void Respawn();
	virtual void OnDead();
	virtual void OnRespawn();
};
