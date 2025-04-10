// Fill out your copyright notice in the Description page of Project Settings.


#include "Machine/Machine.h"

#include "Machine/DropActor.h"


AMachine::AMachine()
{
	PrimaryActorTick.bCanEverTick = false;
	
	FeedPort = CreateDefaultSubobject<UBoxComponent>(TEXT("Feed"));
	GarbageOutlet = CreateDefaultSubobject<UBoxComponent>(TEXT("GarbageOutlet"));
	CornOutlet = CreateDefaultSubobject<UBoxComponent>(TEXT("CornOutlet"));

	//FeedPort->OnComponentBeginOverlap.AddDynamic(this , &AMachine::OnFeedPortBeginOverlap);
	GarbageOutlet->OnComponentBeginOverlap.AddDynamic(this , &AMachine::OnGarbageOutletBeginOverlap);
	CornOutlet->OnComponentBeginOverlap.AddDynamic(this , &AMachine::OnCornOutletBeginOverlap);
	
}

void AMachine::BeginPlay()
{
	Super::BeginPlay();
	

	UClass* Cls = GetDropActorClass();

	for (int i = 0 ; i < 100 ; i ++)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(0, 0, 0));
		SpawnTransform.SetRotation(FQuat(0, 0, 0, 1));
		SpawnTransform.SetScale3D(FVector(1, 1, 1));
		ADropActor* DropActor = Cast<ADropActor>(GetWorld() -> SpawnActor(Cls , &SpawnTransform , SpawnParameters));
		DropPool.Add(DropActor , FDropInfo(DropActor , Idle));
	}

	
}

void AMachine::RecycleDropActor(ADropActor* Actor)
{
	Actor->InRecycle();
	DropPool[Actor].DropState = Idle;
	IdleActors.Add(Actor);
}

void AMachine::ReleaseDropActor()
{
	for (const auto Pair : DropPool)
	{
		if (Pair.Value.DropState == Idle)
		{
			Pair.Key->SetActorLocation(FeedPort->GetComponentLocation());
			
			break;
		}
		 
	}
}

TSubclassOf<AActor> AMachine::GetDropActorClass_Implementation()
{
	return nullptr;
}

void AMachine::OnFeedPortBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ADropActor::StaticClass()))
	{
		return;
	}
}

void AMachine::OnGarbageOutletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ADropActor::StaticClass()))
	{
		return;
	}

	
}

void AMachine::OnCornOutletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ADropActor::StaticClass()))
	{
		return;
	}
}

void AMachine::CalculatePureDegree()
{
	
}

void AMachine::CalculateLossDegree()
{
	
}

// Called every frame
void AMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

