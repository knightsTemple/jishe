// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Machine.generated.h"

class ADropActor;

UENUM()
enum EDropState
{
	Operating,
	Idle
	
};

struct FDropInfo
{
	ADropActor* DropActor;
	EDropState DropState;

	FDropInfo(ADropActor* DropActor,const EDropState DropState) : DropActor(DropActor), DropState(DropState) {}
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

	UFUNCTION()
	void RecycleDropActor(ADropActor* Actor);

	UFUNCTION()
	void ReleaseDropActor();

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
	void CalculatePureDegree();

	UFUNCTION()
	void CalculateLossDegree();

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
