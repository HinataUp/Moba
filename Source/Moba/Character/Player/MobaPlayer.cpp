// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Moba/Character/Controller/MobaPlayerController.h"
#include "Moba/GAS/MobaAbilitySystemComponent.h"
#include "Moba/GAS/AttributeSets/MobaAttributeSet.h"
#include "Moba/UI/OverHeadStatsGauge.h"


// Sets default values
AMobaPlayer::AMobaPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	// CameraBoom->ProbeChannel = ECC_SpringArm;

	ViewCam = CreateDefaultSubobject<UCameraComponent>("View Cam");
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	// 让角色 朝着移动方向旋转身体
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	MobaAsc = CreateDefaultSubobject<UMobaAbilitySystemComponent>("Ability System Component");
	MobaAs = CreateDefaultSubobject<UMobaAttributeSet>("Attribute Set");

	OverheadUIComponent = CreateDefaultSubobject<UWidgetComponent>("UI  Component");
	OverheadUIComponent->SetupAttachment(GetRootComponent());
}

void AMobaPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();
	// get 增强输入系统 并激活
	if (AMobaPlayerController* MobaPC = Cast<AMobaPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSub = MobaPC->GetLocalPlayer()->GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>())
		{
			InputLocalPlayerSub->RemoveMappingContext(InputContext);
			InputLocalPlayerSub->AddMappingContext(InputContext, 0);
		}
	}
}

void AMobaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 绑定 动作类型，触发器（输入产生的触发条件），监听的对象是是谁，具体执行的函数
		InputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMobaPlayer::Jump);
		InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMobaPlayer::HandleMoveInput);
		InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMobaPlayer::HandleLookInput);
	}
}

void AMobaPlayer::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverheadWidget();
}

bool AMobaPlayer::ISLocalControlleredByPlayer() const
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void AMobaPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 初始化ai控制器
	if (NewController && !NewController->IsLocalPlayerController())
	{
		ServerSideInit();
	}
}

void AMobaPlayer::HandleLookInput(const FInputActionValue& InputActionValue)
{
	FVector2D LookValue = InputActionValue.Get<FVector2D>();
	AddControllerPitchInput(-LookValue.Y);
	AddControllerYawInput(LookValue.X);
}

void AMobaPlayer::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D MoveValue = InputActionValue.Get<FVector2D>();
	// 归一化 任何一个方向都是单位移动 ，避免斜角过快
	MoveValue.Normalize();
	AddMovementInput(GetMoveFwdDir() * MoveValue.Y + GetLookRightDir() * MoveValue.X);
}

// EAxis::Y
FVector AMobaPlayer::GetLookRightDir() const
{
	return ViewCam->GetRightVector();
}

// EAxis::X
FVector AMobaPlayer::GetLookFwdDir() const
{
	return ViewCam->GetForwardVector();
}

// 移动前向向量
// 1. 获取前向向量，移除z 轴 分量，只留xy（归一化）
// 2. (右向量 叉积 上向量 UpVector 对应z )得到 第三个向量 （垂直于 原先两个向量的平面，即得到前向向量）
// 左手坐标系， 拇指向右，中指向上，食指向前(前向向量)
FVector AMobaPlayer::GetMoveFwdDir() const
{
	return FVector::CrossProduct(GetLookRightDir(), FVector::UpVector);
}

UAbilitySystemComponent* AMobaPlayer::GetAbilitySystemComponent() const
{
	return MobaAsc;
}

void AMobaPlayer::ConfigureOverheadWidget()
{
	if (!OverheadUIComponent)
	{
		return;
	}
	// 本地玩家 不显示 头顶UI （血量）
	if (ISLocalControlleredByPlayer())
	{
		OverheadUIComponent->SetHiddenInGame(true);
		return;
	}
	if (UOverHeadStatsGauge* OverHeadStatsGauge = Cast<UOverHeadStatsGauge>(OverheadUIComponent->GetUserWidgetObject()))
	{
		OverHeadStatsGauge->ConfigureWithASC(GetAbilitySystemComponent());
		OverheadUIComponent->SetHiddenInGame(false);
		GetWorldTimerManager().ClearTimer(HeadStartGaugeVisibilityUpdateTimerHandle);
		GetWorldTimerManager().SetTimer(HeadStartGaugeVisibilityUpdateTimerHandle, this,
		                                &AMobaPlayer::UpdateHeadGaugeVisibility, HeadStatGaugeVisibilityCheckUpdateGap,
		                                true);
	}
}

void AMobaPlayer::UpdateHeadGaugeVisibility()
{
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (LocalPlayerPawn)
	{
		float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverheadUIComponent->SetHiddenInGame(DistSquared > HeadStatGaugeVisibilityRangeSquared);
	}
}

void AMobaPlayer::ServerSideInit()
{
	MobaAsc->InitAbilityActorInfo(this, this);
	MobaAsc->ApplyInitialEffects();
}

void AMobaPlayer::ClientSideInit()
{
	MobaAsc->InitAbilityActorInfo(this, this);
}
