// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/// Log
///
/// 2021
///
/// September
///
/// 29
///
/// - Cleaned up MapManager
///		- with submethods for LoadNewLevel
///		- closing/opening doors
///		- helpers for point inside map
///	- Door States
///		- Added Opening and Closing states for door as normal classes with TSharedPtr
///		- Added Base InteractableState
///			### Interactable and ButtonTooltip needs some thought
///
/// October
///
/// 3
///
/// - Fixed MapManager walls
///		- better model
///		- Disabled overlap to prevent damaging objects
///		- Added Detailed View toggle to see walls in game
///		- reduced some feathering because walls or more accurate
///	- Cosmic Background Shader
///		- Added gradient function
/// - Perforce
///		- Uploaded a copy