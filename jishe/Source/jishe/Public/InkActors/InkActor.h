// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InkActor.generated.h"

UCLASS()
class JISHE_API AInkActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInkActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	FVector StartPosition;//第一次被点击时的位置坐标

	UPROPERTY()
	float NecessaryNum = 100.0f;
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void SetStaticMeshVisibility(bool Visibility);

	UFUNCTION(BlueprintNativeEvent)
	void EstablishThisActor();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual  void OnMouseChanging(const FVector& NewLocation);

	UFUNCTION()
	void SetStartPosition(const FVector& NewStartPosition);

	UFUNCTION()
	void SetActorNecessaryNum(const float Num);
};
