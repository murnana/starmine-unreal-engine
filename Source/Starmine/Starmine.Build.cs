// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Starmine : ModuleRules
{
	public Starmine(ReadOnlyTargetRules Target) : base(Target)
	{
		// PCH（プリコンパイル済みヘッダ）の使い方。UseExplicitOrSharedPCHs は
		// 各ファイルが明示的に PCH を指定するか、共有 PCH を使う設定。
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Public: このモジュールの .h ファイルからも参照するモジュール。
		// 他のモジュールがこの .h を include するときにも自動でリンクされる。
		// ─ Core / CoreUObject / Engine: UE の基本機能
		// ─ InputCore / EnhancedInput: 入力システム
		// ─ GeometryFramework: UDynamicMeshComponent が入っているモジュール
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GeometryFramework" });

		// Private: .cpp ファイルの中でだけ使うモジュール。
		// ─ GeometryCore: FDynamicMesh3（三角形などのジオメトリデータ構造）が入っているモジュール
		PrivateDependencyModuleNames.AddRange(new string[] { "GeometryCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
