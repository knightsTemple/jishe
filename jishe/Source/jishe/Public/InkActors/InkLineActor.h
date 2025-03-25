// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InkActor.h"
#include "InkLineActor.generated.h"

UCLASS()
class JISHE_API AInkLineActor : public AInkActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInkLineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void OnMouseChanging(const FVector& NewLocation) override;

	virtual void OnMouseLeftClick(const FVector& Location) override;
};
