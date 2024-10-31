// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CatPlatformer : ModuleRules
{
	public CatPlatformer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"EngineSettings",
			"InputCore",
			"ApplicationCore",
			"UMG",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem", 
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"Slate",
			"SlateCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils", 
			/*"OnlineSubsystemSteam",*/
			/*"AdvancedSessions",*/
			/*"AdvancedSteamSessions",*/
			"OnlineSubsystemEOS",
			"EnhancedInput"
		});

		// To include OnlineSubsystemSteam, add it to the plugins section
		// in your uproject file with the Enabled attribute set to true
	}
}
