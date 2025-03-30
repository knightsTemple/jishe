// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkTask.h"

#include "InkActors/InkActorFactory.h"
#include "InkActors/InkInformation.h"

float CalculateLocationFit(const FVector& Loc1, const FVector& Loc2,const float MaxDist = 30) {
	float dist = FVector::Dist(Loc1, Loc2);
	return 1.0f - (dist / MaxDist);
}

// 计算旋转拟合度
float CalculateRotationFit(const FQuat& Rot1, const FQuat& Rot2,const float MaxAngle = 10) {
	// 将四元数转换为旋转器
	const FRotator Rotator1 = Rot1.Rotator();
	const FRotator Rotator2 = Rot2.Rotator();

	// 计算两个旋转器之间的角度差
	FRotator DeltaRotator = Rotator1 - Rotator2;
	DeltaRotator.Normalize();

	float pitchDiff = FMath::Abs(DeltaRotator.Pitch);
	float yawDiff = FMath::Abs(DeltaRotator.Yaw);
	float rollDiff = FMath::Abs(DeltaRotator.Roll);

	// 计算平均角度差
	const float AvgAngleDiff = (pitchDiff + yawDiff + rollDiff) / 3.0f;

	return 1.0f - (AvgAngleDiff / MaxAngle);
}


// 计算缩放拟合度
float CalculateScaleFit(const FVector& Scale1, const FVector& Scale2,const float MaxScaleDiff = 5) {
	FVector scaleDiff = FVector(
		FMath::Abs(Scale1.X - Scale2.X),
		FMath::Abs(Scale1.Y - Scale2.Y),
		FMath::Abs(Scale1.Z - Scale2.Z)
	);
	const float avgDiff = (scaleDiff.X + scaleDiff.Y + scaleDiff.Z) / 3.0f;
	return 1.0f - (avgDiff / MaxScaleDiff);
}

// 综合拟合度
float CalculateOverallFit(float LocationFit, float RotationFit, float ScaleFit, 
						  float LocationWeight, float RotationWeight, float ScaleWeight) {
	return (LocationFit * LocationWeight + RotationFit * RotationWeight + ScaleFit * ScaleWeight) / 
		   (LocationWeight + RotationWeight + ScaleWeight);
}

// Sets default values
AInkTask::AInkTask()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	DataTable = GetInkActorDataTable();
	if (DataTable == nullptr)
	{
		return;
	}
	FString Str = FString("Get All Rows");
	TArray<FInkDatabaseRow*> Rows;
	DataTable->GetAllRows(Str,Rows);
	for (const FInkDatabaseRow* Row : Rows)
	{
		AllTasks.Push(*Row);
	}

	
}

// Called when the game starts or when spawned
void AInkTask::BeginPlay()
{
	Super::BeginPlay();
	
}

UDataTable* AInkTask::GetInkActorDataTable_Implementation()
{
	return nullptr;
}

void AInkTask::GiveTask() const
{
	InkActorFactory->ReceiveThisTask(*DataTable->FindRow<FInkDatabaseRow>(FName(FString::FromInt(NowOperatingWood)), TEXT("GiveMaxTasks")));
}

void AInkTask::ReceiveExaminationInkActors(const TArray<AInkActor*>& Array)
{
	ExaminationInkActors = Array;
}

void AInkTask::TaskCompleted(const TArray<AInkActor*>& NowActors)
{
	if (NowActors.Num() != ExaminationInkActors.Num())
	{
		return;
	}
	bool bLegal = true;
	for (int i = 0 ; i < NowActors.Num() ; i++)
	{
		FVector ActorLocation = NowActors[i]->GetActorLocation();
		FRotator ActorRotation = NowActors[i]->GetActorRotation();
		FVector ActorScale = NowActors[i]->GetActorScale();

		float Fit = CalculateOverallFit(CalculateLocationFit(ActorLocation,ExaminationInkActors[i] -> GetActorLocation()),
							CalculateRotationFit(FQuat(ActorRotation),FQuat(ExaminationInkActors[i] -> GetActorRotation())),
							CalculateScaleFit(ActorScale,ExaminationInkActors[i] -> GetActorScale()),
							0.3f,0.3f,0.3f);

		if (Fit <= 0.8f)
		{
			bLegal = false;
			break;
		}
	}
	if (bLegal)
	{
		TaskSucceed();
	}
	else
	{
		TaskFailed();
	}
}

void AInkTask::TaskFailed()
{
	OnFailed.Broadcast();
}


void AInkTask::TaskSucceed()
{
	NowOperatingWood++;
	if (NowOperatingWood > AllTasks.Num())
	{
		AllTasksCompleted();
		return;
	}
	OnSuccessful.Broadcast();
	
}

void AInkTask::AllTasksCompleted()
{
	OnAllTasksComplete.Broadcast();
}

void AInkTask::TaskChanges()
{
	InkActorFactory->ReceiveThisTask(AllTasks[NowOperatingWood]);
	OnChanged.Broadcast();
}


// Called every frame
void AInkTask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


