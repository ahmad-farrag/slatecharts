// Copyright 2026 Ahmed Farrag. Licensed under the MIT License.

using UnrealBuildTool;

public class SlateChartsLite : ModuleRules
{
	public SlateChartsLite(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UMG"
		});
	}
}
