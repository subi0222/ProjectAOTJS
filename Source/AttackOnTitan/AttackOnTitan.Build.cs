// Unreal Engine Marketplace Copyright

using UnrealBuildTool;

public class AttackOnTitan : ModuleRules
{
	public AttackOnTitan(ReadOnlyTargetRules Target) : base(Target)
	{
        PublicIncludePaths.AddRange(new string[] { "AttackOnTitan" });

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","EnhancedInput", "Niagara", "CableComponent",
	     "AIModule", "GameplayTasks", "NavigationSystem" ,"GameplayTags", "UMG","MotionWarping","GameplayAbilities"});



		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
