// Fill out your copyright notice in the Description page of Project Settings.


#include "Machine/DropActor.h"

#include "Components/BoxComponent.h"
#include "Spatial/MeshAABBTree3.h"


// Sets default values
ADropActor::ADropActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>("MovementComponent");
	
}

// Called when the game starts or when spawned
void ADropActor::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ADropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADropActor::InRecycle()
{
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MovementComponent->GravityScale = 0.0f;
}

void ADropActor::InArise()
{
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MovementComponent->GravityScale = Gravity;
}

