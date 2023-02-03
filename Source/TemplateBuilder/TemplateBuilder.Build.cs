// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TemplateBuilder : ModuleRules
{
	public TemplateBuilder(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"Niagara", 
			"PhysicsCore", 
			"ALSV4_CPP", 
			"GameplayTasks", 
			"UMG", 
			"HTTP", 
			"Json", 
			"OnlineSubsystem", 
			"BaseHelpers", 
			"DlgSystem", 
			"DialogueComponent",
			"Health", 
			"Interaction", 
			"CustomALS", 
			"MenuSystem",
			"WeaponSystem"
			
		});

 		//PublicIncludePaths.AddRange(new string[] {"<ALSV4_CPP-main>/Public", "<ALSV4_CPP-main>/Classes" });
		PrivateDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore"});


	}
}
