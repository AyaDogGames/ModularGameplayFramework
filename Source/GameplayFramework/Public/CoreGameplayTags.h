#pragma once

#include "NativeGameplayTags.h"

// Use our own macro (which adds the module name) to declare the gameplay tags 
#define DA_DECLARE_GAMEPLAY_TAG_EXTERN(TagName) GAMEPLAYFRAMEWORK_API extern FNativeGameplayTag TagName;

// Utility to get leaf tags from a parent tag if it exists in a tag container
static FGameplayTag GetSpecificTag(const FGameplayTagContainer& TagContainer, const FGameplayTag& BaseTag)
{
	// Convert the base tag to a string for prefix searching (e.g., "X.Y")
	FString BaseTagString = BaseTag.ToString();

	// Iterate through the tag container to find a tag that matches the prefix (e.g., "X.Y.Z")
	for (const FGameplayTag& Tag : TagContainer)
	{
		// Check if the tag starts with the base tag (and is longer, e.g., "X.Y.Z")
		if (Tag.ToString().StartsWith(BaseTagString + TEXT(".")))
		{
			return Tag;  // Return the more specific tag if found
		}
	}

	// If no matching tag is found, return an empty/default tag
	return FGameplayTag();
}

namespace CoreGameplayTags
{
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Move);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_LookMouse);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_LookStick);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_PrimaryAttack);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SecondaryAttack);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_PrimaryInteract);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SecondaryInteract);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Use);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Jump);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Run);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Dash);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Crouch);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_InspectZoom);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_InspectRotate);
	
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_ID);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_AI);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_Player);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_Player_ThirdPerson);

	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AbilitySet);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AbilitySetPlayer);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AbilitySetNPC);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AbilitySetItem);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AbilitySetGameState);

	DA_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVital);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalCore);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalCoreHealth);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalCoreMaxHealth);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalCoreMana);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalCoreMaxMana);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalMeta);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalMetaDamage);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesVitalMetaHealing);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesStats);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesStatsPrimary);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(AttributesStatsSecondary);

	DA_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Master);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Sub);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_Sub_Equipped);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(Inventory_ID);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem_Type);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem_Stackable);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem_Consumable);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem_Equipable);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem_ID);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem_EquipAbility);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryItem_Slot);
	
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_Info);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_StatusUpdate);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_Notification);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_Warning);

	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Death);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Death_Dying);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Death_Dead);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Stunned);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Jumping);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Attacking);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Sprinting);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Parrying)
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Healing);
	
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Sprint);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Jump);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_PrimaryAttack);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_SecondaryAttack);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Dash);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Parry);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Heal);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_GainMana);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_SelectItem);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_UseItem);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Buff);

	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Pickup);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Pickup_Health);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Pickup_Mana);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Pickup_Credits);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Pickup_Ability);
	
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Damage);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Heal);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Death);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Montage);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Montage_Projectile);
	
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DA_Damage_Message);

	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cheat_GodMode);
	DA_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cheat_UnlimitedHealth);

}