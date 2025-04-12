// Fill out your copyright notice in the Description page of Project Settings.


#include "Machine/Machine.h"

#include "Machine/DropActor.h"


AMachine::AMachine()
{
	PrimaryActorTick.bCanEverTick = false;
	
	FeedPort = CreateDefaultSubobject<UBoxComponent>(TEXT("Feed"));
	GarbageOutlet = CreateDefaultSubobject<UBoxComponent>(TEXT("GarbageOutlet"));
	CornOutlet = CreateDefaultSubobject<UBoxComponent>(TEXT("CornOutlet"));
	Fan = CreateDefaultSubobject<UBoxComponent>(TEXT("Fan"));
	FeedPort->OnComponentBeginOverlap.AddDynamic(this , &AMachine::OnFeedPortBeginOverlap);
	GarbageOutlet->OnComponentBeginOverlap.AddDynamic(this , &AMachine::OnGarbageOutletBeginOverlap);
	CornOutlet->OnComponentBeginOverlap.AddDynamic(this , &AMachine::OnCornOutletBeginOverlap);
	
}

void AMachine::BeginPlay()
{
	Super::BeginPlay();
	

	UClass* Cls = GetDropActorClass();

	constexpr int InitialBallCount = 200;
	for (int i = 0 ; i < InitialBallCount ; i ++)
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
	IdleActors.Add(Actor); // 将空闲对象添加到数组中
	Actor->SetActorLocation(FeedPort->GetComponentLocation() + FVector{0.f,0.f,1000.f});

	if (IdleActors.Num() == 200 && ReleasedBalls >= TotalBalls)
	{
		OnComplete.Broadcast();
	}
}

void AMachine::ReleaseDropActor(ADropActor* Actor)
{
	IdleActors.Remove(Actor);
	DropPool[Actor].DropState = Operating;
	Actor->MovementComponent->GravityScale = Actor->Gravity;
	Actor->DropState = FMath::RandRange(0.f , 1.f) > GarbageRate ? EDropType::Corn : EDropType::Garbage;
}

void AMachine::CountReleasedDropActor()
{
	ReleasedBalls++;
	if (ReleasedBalls >= TotalBalls)
	{
		for (const auto Actor : IdleActors)
		{
			Actor->MovementComponent->GravityScale = 0.f;
			Actor->GarbageMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Actor->CornMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	ReleaseDropActor(Cast<ADropActor>(OtherActor));
	CountReleasedDropActor();
	
}

void AMachine::OnGarbageOutletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ADropActor::StaticClass()))
	{
		return;
	}
	ADropActor* DropActor = Cast<ADropActor>(OtherActor);
	RecycleDropActor(DropActor);
	if (DropActor->DropState == EDropType::Corn)
	{
		InGarbageCorn++;	
	}
	OnLossDegreeChanged.Broadcast(CalculateLossDegree());
	
}

void AMachine::OnCornOutletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ADropActor::StaticClass()))
	{
		return;
	}
	ADropActor* DropActor = Cast<ADropActor>(OtherActor);
	RecycleDropActor(DropActor);
	InCornBall++;
	if (DropActor->DropState == EDropType::Corn)
	{
		InCornCorn++;
	}
	OnPureDegreeChanged.Broadcast(CalculatePureDegree());
	
}

void AMachine::OnFanBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ADropActor::StaticClass()))
	{
		return;
	}
	ADropActor* DropActor = Cast<ADropActor>(OtherActor);
	DropActor->GiveFanForce(Rate);
}

float AMachine::CalculatePureDegree()
{
	if (InCornBall == 0)
	{
		return 0.0f;
	}
	return static_cast<float>(InCornCorn) / static_cast<float>(InCornBall);
}

float AMachine::CalculateLossDegree()
{
	if (TotalCornNum == 0)
	{
		return  0.0f;
	}
	return static_cast<float>(InGarbageCorn) / static_cast<float>(TotalCornNum);
}

// Called every frame
void AMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMachine::ResetThisMachine()
{
	// 重置计数器
	ReleasedBalls = 0;
	InGarbageCorn = 0;
	InCornBall = 0;
	InCornCorn = 0;

	// 重置小球状态
	for (auto& Pair : DropPool)
	{
		ADropActor* DropActor = Pair.Key;
		FDropInfo& DropInfo = Pair.Value;

		// 回收小球到进料口
		RecycleDropActor(DropActor);
	}
}

void AMachine::ReceiveFanForce(const float NewRate)
{
	Rate = NewRate;
}

