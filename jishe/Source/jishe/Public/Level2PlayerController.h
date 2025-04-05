// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Level2PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JISHE_API ALevel2PlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	void clicked();
private:
	void SetupInputComponent() override;
};
