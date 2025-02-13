// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class pj8 : ModuleRules
{
	public pj8(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"UMG" // Add UMG Module
		});
	}
}
