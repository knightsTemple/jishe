// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkActorFactory.h"

#include "InkActors/InkActor.h"


// Sets default values
AInkActorFactory::AInkActorFactory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInkActorFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInkActorFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInkActorFactory::DeleteAllInkActors()
{
	for (AInkActor* Actor : AllInkActors)
	{
		Actor->Destroy();
	}
	AllInkActors.Empty();
	NowOperatingInkActor->Destroy();
}

void AInkActorFactory::OnMouseChanging(FVector NewLocation) const
{
	NowOperatingInkActor.Get()->OnMouseChanging(NewLocation);
}

void AInkActorFactory::OnMouseLeftClick(EInkActorType InkActorType , FVector Location) 
{
	if (NowOperatingInkActor)
	{
		NowOperatingInkActor.Get()->OnMouseLeftClick(Location);
		NowOperatingInkActor = nullptr;
	}
	else
	{
		
	}
}

