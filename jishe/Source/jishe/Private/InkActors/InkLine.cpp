// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkLine.h"


// Sets default values
AInkLine::AInkLine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInkLine::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInkLine::OnMouseChanging(const FVector& NewLocation)
{
	SetStaticMeshVisibility(true);
	
	const FVector NewPosition = (StartPosition + NewLocation) / 2.0f;
	FRotator NewRotation = FRotator::ZeroRotator;
	const FVector Direction = NewLocation - StartPosition;
	if (Direction.Size() > 0.0f)
	{
		NewRotation = Direction.Rotation();
	}

	const float Distance = FVector::Distance(StartPosition, NewLocation);
	SetActorScale3D(FVector(Distance, 1.0f, 1.0f)); // 线段沿X轴方向
	// Set the new location and rotation
	SetActorLocation(NewPosition);
	SetActorRotation(NewRotation);
}

void AInkLine::OnMouseLeftClick(const FVector& Location)
{
	EstablishThisActor();
}

