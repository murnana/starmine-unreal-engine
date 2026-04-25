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

## Third-Party Asset Rules

All third-party software, fonts, and assets are tracked in `THIRD_PARTY_NOTICES.md` at the repository root.

### Before adding any third-party item

1. Verify the license permits use in a commercial game product.
2. Acceptable licenses: MIT, Apache 2.0, BSD-2-Clause, BSD-3-Clause, OFL-1.1, CC0-1.0, CC-BY-4.0.
3. Requires case-by-case review: GPL (copyleft infection risk), LGPL, CC-BY-SA, proprietary/custom licenses.
4. Do not add items under CC-NC (non-commercial only) or CC-ND (no derivatives).

### When adding a third-party item

Always add an entry to `THIRD_PARTY_NOTICES.md` with:
- **Type** (Font / Library / Asset / etc.)
- **Source** URL
- **License** name and URL
- **Copyright** notice (verbatim from the original)
- Full license text (or a URL reference when the text is very long)

### OFL-1.1 fonts (current: WDXL Lubrifont JP N)

- Bundling inside the game package is permitted.
- Selling the font files standalone is prohibited.
- A Modified Version must not use the original Reserved Font Name.
- `THIRD_PARTY_NOTICES.md` must be included in every distribution of the game.
- OFL fonts must remain under OFL-1.1; relicensing is prohibited.

### Distribution requirement

`THIRD_PARTY_NOTICES.md` must be packaged and shipped with every build of the game.
