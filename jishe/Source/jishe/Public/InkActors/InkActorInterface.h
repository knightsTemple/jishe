// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InkActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UInkActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JISHE_API IInkActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnLeftMouseButtonDown(FHitResult HitLocation) = 0;
};
