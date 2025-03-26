// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkActor.h"


// Sets default values
AInkActor::AInkActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInkActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInkActor::SetStaticMeshVisibility_Implementation(bool Visibility)
{}

void AInkActor::EstablishThisActor_Implementation()
{
	SetStaticMeshVisibility(true);
}

// Called every frame
void AInkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInkActor::OnMouseChanging(const FVector& NewLocation)
{}

void AInkActor::OnMouseLeftClick(const FVector& Location)
{}

void AInkActor::SetStartPosition(const FVector& NewStartPosition)
{
	StartPosition = NewStartPosition;
}

