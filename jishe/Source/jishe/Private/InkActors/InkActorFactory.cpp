// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkActorFactory.h"

#include "Customizations/MathStructProxyCustomizations.h"
#include "InkActors/InkActor.h"
#include "InkActors/InkTask.h"
#include "InkActors/Wood.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AInkActorFactory::AInkActorFactory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InkLineClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/InkActors/BP_InkLine")).Class;
	InkCircleClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Game/InkActors/BP_InkCircle")).Class;
	check(InkLineClass);
	check(InkCircleClass);
	NowOperatingInkActor = nullptr;

	NowInkTaskClass = Cast<AInkTask>(UGameplayStatics::GetActorOfClass(GetWorld() , AInkTask::StaticClass()));
}

// Called when the game starts or when spawned
void AInkActorFactory::BeginPlay()
{
	Super::BeginPlay();
	
}


void AInkActorFactory::ReceiveThisTask(const FInkDatabaseRow& ThisTask)
{
	NowTask = ThisTask;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator SpawnRotation = FRotator(0, 0, 0);
	NowWood->Destroy();
	NowWood = Cast<AWood>(GetWorld()->SpawnActor(NowTask.WoodType.Get() , &WoodSpawnLocation , &SpawnRotation, SpawnParameters));
	GenerateExaminationInkActors();
}

void AInkActorFactory::	GenerateExaminationInkActors()
{
	TArray<AInkActor*> ExaminationInkActors;
	for (const auto [InkActorType, InkActorTransform] : NowTask.InkData.InkActorTransforms)
	{
		TSubclassOf<AInkActor> InkActorClass;
		switch (InkActorType)
		{
		case EInkActorType::Line: InkActorClass = InkLineClass; break;
		case EInkActorType::Circle: InkActorClass = InkCircleClass; break;
		}
		const FVector Location = InkActorTransform.GetLocation() + NowWood->GetActorLocation() + NowWood->Box->GetScaledBoxExtent();
		const FRotator Rotator(InkActorTransform.GetRotation());
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AInkActor* InkActor = Cast<AInkActor>(GetWorld()->SpawnActor(InkActorClass, &Location , &Rotator , SpawnParameters));
		InkActor->SetActorScale3D(InkActorTransform.GetScale3D());

		ExaminationInkActors.Add(InkActor);
	}
	NowInkTaskClass->ReceiveExaminationInkActors(ExaminationInkActors);
}

// Called every frame
void AInkActorFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInkActorFactory::DeleteAllInkActors()
{
	for (AInkActor* Actor : AllInkActors)
	{
		Actor->Destroy();
	}
	AllInkActors.Empty();
	if (NowOperatingInkActor)
	{
		NowOperatingInkActor->Destroy();
	}
	
}

void AInkActorFactory::DeleteLastActor()
{
	if (AllInkActors.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,"There is nothing to delete");
		return;
	}
	if (NowOperatingInkActor)
	{
		NowOperatingInkActor->Destroy();
		return;
	}
	AInkActor* CurActor =  AllInkActors.Pop();
	CurActor->Destroy();
}

void AInkActorFactory::OnMouseChanging(const FVector& NewLocation) 
{
	if (NowOperatingInkActor)
	{
		NowOperatingInkActor->OnMouseChanging(NewLocation);
	}
}

void AInkActorFactory::OnMouseLeftClick(EInkActorType InkActorType , const FVector& Location) 
{
	if (NowOperatingInkActor)
	{
		NowOperatingInkActor -> EstablishThisActor();
		AllInkActors.Add(NowOperatingInkActor);
		NowOperatingInkActor = nullptr;
		//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,"Establish An Actor");

		if (NowInkTaskClass && 
			AllInkActors.Num() == NowTask.InkData.InkActorTransforms.Num())
		{
			NowInkTaskClass->TaskCompleted(AllInkActors);
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,"Create An Actor");
		TSubclassOf<AInkActor> InkActorClass ;
		switch (InkActorType)
		{
		case EInkActorType::Line : InkActorClass = InkLineClass; break;
		case EInkActorType::Circle : InkActorClass = InkCircleClass; break;
		}

		FActorSpawnParameters SpawnParams ;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FRotator RotatorSpawnParams = {0,0,0};
		NowOperatingInkActor = Cast<AInkActor>(GetWorld()->SpawnActor(InkActorClass,&Location,&RotatorSpawnParams,SpawnParams));
		
		//GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("%f,%f") , Location.X,Location.Y));
		
		NowOperatingInkActor ->SetStaticMeshVisibility(false);
		NowOperatingInkActor ->SetStartPosition(Location);
		NowOperatingInkActor->SetActorNecessaryNum(InkActorType == EInkActorType::Circle ? CircleRadiusDelta : LineWidth);
		
	}
}

