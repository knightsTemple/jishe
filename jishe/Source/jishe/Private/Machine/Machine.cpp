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

	BallsPerSecond = TotalBalls / ReleasedTime;
	ReleaseInterval = 1.0f / BallsPerSecond;

	// 启动计时器，开始释放小球
	GetWorld()->GetTimerManager().SetTimer(ReleaseTimerHandle, this, &AMachine::ReleaseBalls, ReleaseInterval, true);
}

	


void AMachine::RecycleDropActor(ADropActor* Actor)
{
	Actor->InRecycle();
	DropPool[Actor].DropState = Idle;
	IdleActors.Add(Actor); // 将空闲对象添加到数组中
}

void AMachine::ReleaseDropActor()
{
	if (IdleActors.Num() > 0)
	{
		ADropActor* DropActor = IdleActors.Pop(); // 从空闲数组中取出一个对象
		DropPool[DropActor].DropState = Operating;
		DropActor->SetActorLocation(FeedPort->GetComponentLocation());
	}
	else
	{
		// 如果没有空闲对象，动态创建新的对象
		UClass* Cls = GetDropActorClass();
		if (Cls)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FeedPort->GetComponentLocation());
			SpawnTransform.SetRotation(FQuat(0, 0, 0, 1));
			SpawnTransform.SetScale3D(FVector(1, 1, 1));
			ADropActor* NewDropActor = Cast<ADropActor>(GetWorld()->SpawnActor(Cls, &SpawnTransform, SpawnParameters));
			DropPool.Add(NewDropActor, FDropInfo(NewDropActor, Operating));
		}
	}
}

void AMachine::ReleaseBalls()
{
	if (ReleasedBalls >= TotalBalls || RemainingTime <= 0.0f)
	{
		// 所有小球已经释放完毕或时间结束，停止计时器
		GetWorld()->GetTimerManager().ClearTimer(ReleaseTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("All balls released or time ended!"));
		return;
	}

	// 每次释放一定数量的小球
	int32 BallsToRelease = FMath::Min(BallsPerSecond, TotalBalls - ReleasedBalls);

	for (int i = 0; i < BallsToRelease; i++)
	{
		ReleaseDropActor();
		ReleasedBalls++;
	}

	UE_LOG(LogTemp, Log, TEXT("Released Balls: %d, Remaining Time: %f"), ReleasedBalls, RemainingTime);

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
	ADropActor* DropActor = Cast<ADropActor>(OtherActor);
	RecycleDropActor(DropActor);
	
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

