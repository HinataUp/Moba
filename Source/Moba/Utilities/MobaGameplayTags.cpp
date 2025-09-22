#include "MobaGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace FMobaGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_BasicAttack, "Ability.BasicAttack", "基础攻击能力标签，用于标识普通攻击技能");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_BasicAttack_Pressed, "Ability.BasicAttack.Pressed", "基础攻击按键按下事件，触发攻击起始动作");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_BasicAttack_Released, "Ability.BasicAttack.Released", "基础攻击按键释放事件，触发攻击执行动作");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stats_Dead, "Stats.Dead", "死亡状态标签，标识角色当前处于死亡状态");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stats_Stun, "Stats.Stun", "眩晕状态标签，标识角色当前处于被眩晕状态");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stats_Aim, "Stats.Aim", "瞄准状态标签，标识角色当前处于瞄准姿态");


	// These are mapped to the movement modes inside GetMovementModeTagMap()
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking, "Movement.Mode.Walking", "Default Character movement tag");

	// Unreal Movement Modes
	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{MOVE_Walking, Movement_Mode_Walking},
	};

	// Custom Movement Modes
	const TMap<uint8, FGameplayTag> CustomMovementModeTagMap =
	{
		// Fill these in with your custom modes
	};

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogTemp, Display,
					       TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."),
					       *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
}
