// Fill out your copyright notice in the Description page of Project Settings.


#include "Machine/MachineTask.h"

#include "Kismet/GameplayStatics.h"
#include "Machine/Machine.h"


// Sets default values
AMachineTask::AMachineTask()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CropMap.Add(ECropType::Crop ,false);
	CropMap.Add(ECropType::Rice ,false);
	CropMap.Add(ECropType::Rapeseed ,false);
	
}

// Called when the game starts or when spawned
void AMachineTask::BeginPlay()
{
	Super::BeginPlay();

	AMachine* Actor = Cast<AMachine>(UGameplayStatics::GetActorOfClass(GetWorld() , AMachine::StaticClass()));
	Actor->OnComplete.AddDynamic(this , &AMachineTask::OneTaskComplete);
}

// Called every frame
void AMachineTask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMachineTask::ChangeCornType(ECropType Type)
{
	NowCropType = Type;
}

void AMachineTask::OneTaskComplete()
{
	CropMap[NowCropType] = true;
	bool bAllDown = true;
	for (const auto Pair : CropMap)
	{
		if (!Pair.Value) 
		{
			bAllDown = false;
			break;
		}
	}
	if (bAllDown)
	{
		OnAllTasksComplete.Broadcast();
	}
}

