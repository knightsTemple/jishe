// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Machine.generated.h"

class ADropActor;

UENUM(BlueprintType)
enum EDropRunningType : uint8
{
	Operating,
	Idle
	
};

USTRUCT(BlueprintType)
struct FDropInfo
{
	GENERATED_BODY()
	
	TObjectPtr<ADropActor> DropActor;
	EDropRunningType DropState;

	FDropInfo(ADropActor* DropActor,const EDropRunningType DropState) : DropActor(DropActor), DropState(DropState) {}
	FDropInfo() : DropActor(nullptr), DropState(Idle) {}
};

UCLASS()
class JISHE_API AMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	TMap<ADropActor* , FDropInfo> DropPool;

	UPROPERTY(BlueprintReadOnly)
	TArray<ADropActor*> IdleActors;

	UPROPERTY(BlueprintReadWrite , EditAnywhere)
	float ReleasedTime = 20.f;

	float RemainingTime = 20.0f; // 剩余时间（秒）
	
	int ReleasedBalls = 0;     // 已经释放的小球数量

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int TotalCornNum;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int TotalBalls = 1000;     // 总共需要释放的小球数量
	
	int InGarbageCorn = 0;

	int InCornBall = 0;

	int InCornCorn = 0;
	
	//计时器句柄
	FTimerHandle ReleaseTimerHandle;

	//每帧释放的小球数量
	int BallsPerSecond = TotalBalls / ReleasedTime; //每帧释放的小球数量， 假设每秒释放50个小球
	
	float ReleaseInterval = 1.0f / BallsPerSecond; // 每帧释放的间隔时间


	UFUNCTION()
	void RecycleDropActor(ADropActor* Actor);

	UFUNCTION()
	void ReleaseDropActor();

	UFUNCTION()
	void ReleaseBalls();

	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<AActor> GetDropActorClass();
	
	UFUNCTION()
	void OnFeedPortBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnGarbageOutletBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnCornOutletBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	float CalculatePureDegree();

	UFUNCTION()
	float CalculateLossDegree();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	UBoxComponent* FeedPort;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	UBoxComponent* GarbageOutlet;

	UPROPERTY(EditAnywhere , BlueprintReadOnly)
	UBoxComponent* CornOutlet;

	UPROPERTY(BlueprintReadOnly)
	float LossDegree = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float PureDegree = 0.0f;
};
