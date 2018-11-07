// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SimTech_CrowdEditorTarget : TargetRules
{
	public SimTech_CrowdEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "SimTech_Crowd" } );
	}
}
