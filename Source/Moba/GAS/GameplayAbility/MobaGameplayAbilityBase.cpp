// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaGameplayAbilityBase.h"

UAnimInstance* UMobaGameplayAbilityBase::GetOwnerAnimInstance() const
{
	if (USkeletalMeshComponent* OwnerSkeletalMeshComp = GetOwningComponentFromActorInfo())
	{
		return OwnerSkeletalMeshComp->GetAnimInstance();
	}
	return nullptr;
}
