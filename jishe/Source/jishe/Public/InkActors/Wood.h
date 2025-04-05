// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Wood.generated.h"

UCLASS()
class JISHE_API AWood : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	USceneComponent* Root;
};
