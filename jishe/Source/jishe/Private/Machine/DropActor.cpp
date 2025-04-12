// Fill out your copyright notice in the Description page of Project Settings.


#include "Machine/DropActor.h"

#include "Components/BoxComponent.h"
#include "Spatial/MeshAABBTree3.h"


// Sets default values
ADropActor::ADropActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GarbageMesh = CreateDefaultSubobject<UStaticMeshComponent>("GarbageMesh");
	CornMesh = CreateDefaultSubobject<UStaticMeshComponent>("CornMesh");
	GarbageMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CornMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
	GarbageMesh->SetVisibility(false);
	GarbageMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CornMesh->SetVisibility(false);
	CornMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//MovementComponent->GravityScale = 0.0f;
}

void ADropActor::InArise()
{
	GarbageMesh->SetVisibility(true);
	GarbageMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GarbageMesh->SetVisibility(true);
	GarbageMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MovementComponent->GravityScale = Gravity;
}

void ADropActor::ChangeState()
{
	switch (DropState)
	{
	case EDropType::Corn :
		GarbageMesh->SetVisibility(false);break;
	case EDropType::Garbage:
		CornMesh->SetVisibility(false);break;
	}
}

void ADropActor::GiveFanForce(const float Rate)
{
	switch (DropState)
	{
	case EDropType::Corn :
		MovementComponent->AddImpulse(FVector(CornImpulse + Rate, 0, 0));
	case EDropType::Garbage:
		MovementComponent->AddImpulse(FVector(GarbageImpulse + Rate, 0, 0));
		
	}
}

