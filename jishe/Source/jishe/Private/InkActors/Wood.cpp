// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/Wood.h"

#include "Misc/MapErrors.h"


// Sets default values
AWood::AWood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMesh->SetupAttachment(Root);
	Box = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	Box->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AWood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

