// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MachineTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllTasksComplete);

UENUM(BlueprintType)
enum class ECropType : uint8
{
	Crop,
	Rice,
	Rapeseed //油菜籽
};

UCLASS()
class JISHE_API AMachineTask : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMachineTask();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TMap<ECropType, bool> CropMap;
	
	UPROPERTY(BlueprintAssignable)
	FOnAllTasksComplete OnAllTasksComplete;

	UPROPERTY(BlueprintReadOnly)
	ECropType NowCropType = ECropType::Rice;

	UFUNCTION(BlueprintCallable)
	void ChangeCornType(ECropType Type);

	UFUNCTION(BlueprintCallable)
	void OneTaskComplete();

	
};
