// Unreal Engine Marketplace Copyright

using UnrealBuildTool;
using System.Collections.Generic;

public class AttackOnTitanEditorTarget : TargetRules
{
	public AttackOnTitanEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "AttackOnTitan" } );
	}
}
