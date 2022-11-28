// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;
 
public class HomespaceMetaverse : ModuleRules
{
    public static bool IsDLSSPluginEnabled(ReadOnlyTargetRules Target)
    {
        var Project = ProjectDescriptor.FromFile(Target.ProjectFile);
        foreach (PluginReferenceDescriptor PluginReference in Project.Plugins)
        {
            if (String.Compare(PluginReference.Name, "DLSS", true) == 0)
            {
                return PluginReference.IsEnabledForPlatform(Target.Platform) && PluginReference.IsEnabledForTargetConfiguration(Target.Configuration) && PluginReference.IsEnabledForTarget(Target.Type);
            }
        }
        return false;
    }

    public HomespaceMetaverse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
            "PhysicsCore",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "SocketIOClient", 
			"SocketIOLib",
            "EngineSettings",
            "Networking",
            "DeveloperSettings",
            "Json", 
			"SIOJson",
			"Slate",
            "Niagara",
            "UMG",
            "SlateCore",
            "Blu",
            "BluLoader"
		});

        PrivateDependencyModuleNames.AddRange(new string[] {
            "InputCore",
            "RHI",
            "Renderer",
            "RenderCore",
         });

        // if RenderDoc plugin crashes try to disable DLSS
        if (IsDLSSPluginEnabled(Target))
        {
            PublicDependencyModuleNames.Add("DLSSBlueprint");
            PublicDefinitions.Add("HS_WITH_DLSS=1");
        }

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "HomespaceMetaverse"
            }
        );

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
