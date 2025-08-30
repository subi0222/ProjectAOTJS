// Unreal Engine Marketplace Copyright

using UnrealBuildTool;
using System.Collections.Generic;

public class AttackOnTitanTarget : TargetRules
{
	public AttackOnTitanTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "AttackOnTitan" } );
	}
}
