// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InkActorFactory.generated.h"

class AInkActor;

UENUM(BlueprintType)
enum class EInkActorType : uint8
{
	Line,
	Circle
};


UCLASS()
class JISHE_API AInkActorFactory : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInkActorFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AInkActor*> AllInkActors;//目前创建过的InkActor

	UPROPERTY()
	TObjectPtr<AInkActor> NowOperatingInkActor;//现在正在操作的InkActor

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void DeleteAllInkActors();

	UFUNCTION()
	void OnMouseChanging(FVector NewLocation) const;

	UFUNCTION()
	void OnMouseLeftClick(EInkActorType InkActorType , FVector Location) ;
};
