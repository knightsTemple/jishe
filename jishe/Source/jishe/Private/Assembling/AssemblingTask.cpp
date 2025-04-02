#include "Assembling/AssemblingTask.h"

#include "Assembling/MainComponents.h"
#include "Kismet/GameplayStatics.h"

AAssemblingTask::AAssemblingTask()
{
	NowDataTable = Cast<UDataTable>(FSoftObjectPath(TEXT("/Game/AssemblingActors/DT_AssemblingTasks.DT_AssemblingTasks")).TryLoad());
	NowMainComponents = Cast<AMainComponents>(UGameplayStatics :: GetActorOfClass(GetWorld() , AMainComponents::StaticClass()));
	TArray<FAssemblingTasks*> Tasks;
	NowDataTable->GetAllRows(TEXT("GetTasks") , Tasks);
	for (const auto& Task : Tasks)
	{
		NowTasks.Push(*Task);
	}
}

void AAssemblingTask::BeginPlay()
{
	Super::BeginPlay();
}


void AAssemblingTask::AllDone()
{
	OnFinished.Broadcast();
}

void AAssemblingTask::OneTaskDone()
{
	NowTask++;
	if(NowTask == NowTasks.Num())
	{
		AllDone();
		return;
	}
	NowMainComponents->Destroy();
	NowMainComponents = Cast<AMainComponents>(GetWorld() -> SpawnActor(NowTasks[NowTask].ComponentClass , &SpawnLocation));
	OnChanged.Broadcast();
}
