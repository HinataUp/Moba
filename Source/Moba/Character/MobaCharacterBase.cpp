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
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


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
	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		"Perception Stimuli Source Component");
}

// Called when the game starts or when spawned
void AMobaCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverheadWidget();
	MeshRelativeTransform = GetMesh()->GetRelativeTransform();
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
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
}

void AMobaCharacterBase::ClientSideInit()
{
	MobaAsc->InitAbilityActorInfo(this, this);
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

void AMobaCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMobaCharacterBase, TeamID);
}

void AMobaCharacterBase::BindGASChangeDelegates()
{
	if (MobaAsc)
	{
		MobaAsc->RegisterGameplayTagEvent(FMobaGameplayTags::Stats_Dead).
		         AddUObject(this, &AMobaCharacterBase::DeathTagUpdated);
	}
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
		float MontageDuration = PlayAnimMontage(DeathMontage);
		GetWorldTimerManager().SetTimer(DeathMontageTimerHandle, this,
		                                &AMobaCharacterBase::DeathMontageFinished,
		                                MontageDuration + DeathMontageFinishTimeShift);
	}
}

void AMobaCharacterBase::StartDeathSequence()
{
	OnDead();
	PlayDeathAnimation();
	SetStatusGaugeEnabled(false);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetAIPerceptionStimuliSourceEnabled(false); // 死亡禁用 AI 视觉
}


// 重生后 启用 物理碰撞查询 以及 设置人物 恢复为 walk 状态
// 禁用蒙太奇动画，并重新启用 头顶UI血条现实
// 默认重生 启用AI 视觉组件功能
void AMobaCharacterBase::Respawn()
{
	OnRespawn();
	SetRagdollEnabled(false);
	SetAIPerceptionStimuliSourceEnabled(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	SetStatusGaugeEnabled(true);

	// 复活 重生点
	if (HasAuthority() && GetController())
	{
		TWeakObjectPtr<AActor> StartSpot = GetController()->StartSpot;
		if (StartSpot.IsValid())
		{
			SetActorTransform(StartSpot->GetActorTransform());
		}
	}

	if (MobaAsc)
	{
		MobaAsc->ApplyFullStatEffect();
	}
}

void AMobaCharacterBase::OnDead()
{
}

void AMobaCharacterBase::OnRespawn()
{
}

void AMobaCharacterBase::DeathMontageFinished()
{
	SetRagdollEnabled(true);
}

void AMobaCharacterBase::SetRagdollEnabled(bool bIsEnabled)
{
	if (bIsEnabled)
	{
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(MeshRelativeTransform);
	}
}

void AMobaCharacterBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId AMobaCharacterBase::GetGenericTeamId() const
{
	return TeamID;
}

void AMobaCharacterBase::SetAIPerceptionStimuliSourceEnabled(bool bIsEnabled)
{
	if (!PerceptionStimuliSourceComponent)
	{
		return;
	}

	if (bIsEnabled)
	{
		PerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	}
	else
	{
		PerceptionStimuliSourceComponent->UnregisterFromPerceptionSystem();
	}
}
