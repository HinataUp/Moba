#pragma once

#include "NativeGameplayTags.h"

namespace FMobaGameplayTags
{
	MOBA_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all of the custom native tags that Lyra will use
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_BasicAttack);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_BasicAttack_Pressed);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_BasicAttack_Released);

	// combo
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combo_Change);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combo_Change_Combo01);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combo_Change_Combo02);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combo_Change_Combo03);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combo_Change_Combo04);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combo_Change_End);

	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combo_Damage);
	

	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stats_Dead);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stats_Stun);
	MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stats_Aim);


	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	MOBA_API extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	MOBA_API extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	// MOBA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
};
