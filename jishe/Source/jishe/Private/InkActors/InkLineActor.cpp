// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkLineActor.h"


// Sets default values
AInkLineActor::AInkLineActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInkLineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInkLineActor::OnMouseChanging(const FVector& NewLocation)
{
	SetStaticMeshVisibility(true);	
	const FVector NewPosition = (StartPosition + NewLocation) / 2.0f;
	FRotator NewRotation = FRotator::ZeroRotator;
	const FVector Direction = NewLocation - StartPosition;
	if (Direction.Size() > 0.0f)
	{
		NewRotation = Direction.Rotation();
	}

	// Set the new location and rotation
	SetActorLocation(NewPosition);
	SetActorRotation(NewRotation);
}

void AInkLineActor::OnMouseLeftClick(const FVector& Location)
{
	EstablishThisActor();
}

