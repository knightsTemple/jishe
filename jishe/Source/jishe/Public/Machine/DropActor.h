// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DropActor.generated.h"


UENUM(BlueprintType)
enum class EDropType : uint8
{
	Corn,
	Garbage
};

UCLASS()
class JISHE_API ADropActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADropActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float Gravity = 0.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY()
	EDropType DropState;
	
	UFUNCTION()
	void InRecycle();

	UFUNCTION()
	void InArise();
};
