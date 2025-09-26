// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaCharacterBase.h"

#include "GameplayTagContainer.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Moba/GAS/MobaAbilitySystemComponent.h"
#include "Moba/GAS/AttributeSets/MobaAttributeSet.h"
#include "Moba/UI/OverHeadStatsGauge.h"
#include "Moba/Utilities/MobaGameplayTags.h"


// Sets default values
AMobaCharacterBase::AMobaCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MobaAsc = CreateDefaultSubobject<UMobaAbilitySystemComponent>("Ability System Component");
	MobaAs = CreateDefaultSubobject<UMobaAttributeSet>("Attribute Set");

	OverheadUIComponent = CreateDefaultSubobject<UWidgetComponent>("UI  Component");
	OverheadUIComponent->SetupAttachment(GetRootComponent());

	BindGASChangeDelegates();
}

// Called when the game starts or when spawned
void AMobaCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverheadWidget();
}

// Called every frame
void AMobaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMobaCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


UAbilitySystemComponent* AMobaCharacterBase::GetAbilitySystemComponent() const
{
	return MobaAsc;
}

void AMobaCharacterBase::DeathTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount != 0)
	{
		StartDeathSequence();
	}
	else
	{
		Respawn();
	}
}

void AMobaCharacterBase::ConfigureOverheadWidget()
{
	if (!OverheadUIComponent)
	{
		return;
	}
	// 本地玩家 不显示 头顶UI （血量）
	if (IsLocalControlledByPlayer())
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
		                                &AMobaCharacterBase::UpdateHeadGaugeVisibility,
		                                HeadStatGaugeVisibilityCheckUpdateGap,
		                                true);
	}
}

void AMobaCharacterBase::ServerSideInit()
{
	MobaAsc->InitAbilityActorInfo(this, this);
	MobaAsc->ApplyInitialEffects();
	MobaAsc->GiveInitialAbilities();
}

void AMobaCharacterBase::ClientSideInit()
{
	MobaAsc->InitAbilityActorInfo(this, this);
}

void AMobaCharacterBase::PawnClientRestart()
{
	Super::PawnClientRestart();
}

bool AMobaCharacterBase::IsLocalControlledByPlayer() const
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void AMobaCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 初始化ai控制器
	if (NewController && !NewController->IsLocalPlayerController())
	{
		ServerSideInit();
	}
}

void AMobaCharacterBase::BindGASChangeDelegates()
{
	if (MobaAsc)
	{
		MobaAsc->RegisterGameplayTagEvent(FMobaGameplayTags::Stats_Dead).
		         AddUObject(this, &AMobaCharacterBase::DeathTagUpdated);
	}
}

void AMobaCharacterBase::StartDeathSequence()
{
	OnDead();
	PlayDeathAnimation();
	SetStatusGaugeEnabled(false);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMobaCharacterBase::UpdateHeadGaugeVisibility() const
{
	if (APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverheadUIComponent->SetHiddenInGame(DistSquared > HeadStatGaugeVisibilityRangeSquared);
	}
}

void AMobaCharacterBase::SetStatusGaugeEnabled(bool bIsEnabled)
{
	GetWorldTimerManager().ClearTimer(HeadStartGaugeVisibilityUpdateTimerHandle);
	if (bIsEnabled)
	{
		ConfigureOverheadWidget();
	}
	else
	{
		OverheadUIComponent->SetHiddenInGame(true);
	}
}

void AMobaCharacterBase::PlayDeathAnimation()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
}


void AMobaCharacterBase::Respawn()
{
	OnRespawn();
}

void AMobaCharacterBase::OnDead()
{
}

void AMobaCharacterBase::OnRespawn()
{
}
