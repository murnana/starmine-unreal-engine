# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Starmine is an Unreal Engine 5.7 C++ game project targeting Mac and Windows. It uses the Enhanced Input system. The project is in early stages with a single runtime module.

## Build Commands

Build (game target):
```
/path/to/UnrealBuildTool Starmine Mac Development
```

Build (editor target):
```
/path/to/UnrealBuildTool StarmineEditor Mac Development
```

The actual UBT path depends on the local Unreal Engine installation. On macOS, it is typically found at:
`/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh`

Alternatively, open the `.uproject` in Unreal Editor and build from there, or use the generated Xcode workspace (`Starmine (Mac).xcworkspace`).

## Architecture

- **Engine**: Unreal Engine 5.7, C++ with `BuildSettingsVersion.V6`
- **Module**: Single module `Starmine` (Runtime, Default loading phase)
- **Dependencies**: Core, CoreUObject, Engine, InputCore, EnhancedInput
- **Targets**: `Starmine.Target.cs` (Game), `StarmineEditor.Target.cs` (Editor)
- **Source**: All C++ source lives under `Source/Starmine/`
- **Build config**: `Source/Starmine/Starmine.Build.cs`

## Conventions

- Follow Unreal Engine coding standards (e.g., `F` prefix for structs, `U` for UObject-derived, `A` for AActor-derived, `E` for enums)
- Use Enhanced Input (`UInputAction`, `UInputMappingContext`) rather than legacy input bindings
- PCH usage mode is `UseExplicitOrSharedPCHs`
