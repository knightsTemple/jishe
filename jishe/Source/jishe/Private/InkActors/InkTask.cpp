// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkTask.h"

#include "InkActors/InkActorFactory.h"
#include "InkActors/InkCircle.h"
#include "InkActors/InkInformation.h"
#include "InkActors/InkLine.h"
#include "Kismet/GameplayStatics.h"

float CalculateLocationFit(const FVector& Loc1, const FVector& Loc2,const float MaxDist = 30) {
	float dist = FVector::Dist(Loc1, Loc2);
	return 1.0f - (dist / MaxDist);
}

// 计算旋转拟合度
float CalculateRotationFit(const FRotator& Rotator1, const FRotator& Rotator2, float MaxAngle = 10) {
	// 计算两个旋转器之间的角度差
	FRotator DeltaRotator = Rotator1 - Rotator2;

	// 将角度差限制在-180到180的范围内
	DeltaRotator.Pitch = FMath::Fmod(DeltaRotator.Pitch, 360.0f);
	DeltaRotator.Yaw = FMath::Fmod(DeltaRotator.Yaw, 360.0f);
	DeltaRotator.Roll = FMath::Fmod(DeltaRotator.Roll, 360.0f);

	// 确保角度差在-180到180的范围内
	DeltaRotator.Pitch = FMath::Abs(DeltaRotator.Pitch) > 180.0f ? 360.0f - FMath::Abs(DeltaRotator.Pitch) : DeltaRotator.Pitch;
	DeltaRotator.Yaw = FMath::Abs(DeltaRotator.Yaw) > 180.0f ? 360.0f - FMath::Abs(DeltaRotator.Yaw) : DeltaRotator.Yaw;
	DeltaRotator.Roll = FMath::Abs(DeltaRotator.Roll) > 180.0f ? 360.0f - FMath::Abs(DeltaRotator.Roll) : DeltaRotator.Roll;

	// 计算绝对值
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
						  float LocationWeight = 0.3, float RotationWeight = 0.3, float ScaleWeight = 0.3) {
	return (LocationFit * LocationWeight + RotationFit * RotationWeight + ScaleFit * ScaleWeight) / 
		   (LocationWeight + RotationWeight + ScaleWeight);
}

float CalculatePositionFit(const FVector& Center1, const FVector& Center2, float MaxDist = 30) {
	float dist = FVector::Dist(Center1, Center2);
	return 1.0f - (dist / MaxDist);
}

// 计算半径拟合度
float CalculateRadiusFit(float Radius1, float Radius2, float MaxRadiusDiff = 30) {
	float diff = FMath::Abs(Radius1 - Radius2);
	return 1.0f - (diff / MaxRadiusDiff);
}

// 计算形状拟合度
float CalculateShapeFit(const FVector& Center1, float Radius1, const FVector& Center2, float Radius2, float MaxShapeDiff = 10) {
	// 计算两个圆之间的相对位置和大小差异
	float dist = FVector::Dist(Center1, Center2);
	float radiusSum = Radius1 + Radius2;
	float shapeDiff = FMath::Abs(dist - radiusSum);

	return 1.0f - (shapeDiff / MaxShapeDiff);
}

// 综合拟合度
float CalculateCirCleOverallFit(float PositionFit, float RadiusFit, float ShapeFit, 
						  float PositionWeight, float RadiusWeight, float ShapeWeight) {
	return (PositionFit * PositionWeight + RadiusFit * RadiusWeight + ShapeFit * ShapeWeight) / 
		   (PositionWeight + RadiusWeight + ShapeWeight);
}



// Sets default values
AInkTask::AInkTask()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInkTask::BeginPlay()
{
	Super::BeginPlay();
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
	InkActorFactory =Cast<AInkActorFactory>( UGameplayStatics::GetActorOfClass(GetWorld() , AInkActorFactory::StaticClass()));
	GiveTask();
	
}

UDataTable* AInkTask::GetInkActorDataTable_Implementation()
{
	return nullptr;
}

void AInkTask::GiveTask() const
{
	if (DataTable == nullptr)
	{
		return;
	}
	FInkDatabaseRow Row = *DataTable->FindRow<FInkDatabaseRow>(FName(FString::FromInt(NowOperatingWood + 1)), TEXT("GiveMaxTasks"));
	InkActorFactory->ReceiveThisTask(Row);
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

		auto CalculateFit = [&]()-> float
		{
			if (NowActors[i]->StaticClass() != ExaminationInkActors[i]->StaticClass())
			{
					return 0.0f;
			}
			if (NowActors[i]->StaticClass() == AInkCircle::StaticClass())
			{
				return CalculateCirCleOverallFit(
						CalculatePositionFit(NowActors[i]->GetActorLocation(),ExaminationInkActors[i]->GetActorLocation(),EvaluationValueParameters.MaxDist),
						CalculateRadiusFit(Cast<AInkCircle>(NowActors[i])->InnerRadius , Cast<AInkCircle>(ExaminationInkActors[i]) -> InnerRadius , EvaluationValueParameters.MaxRadiusDiff),
						CalculateShapeFit(ActorLocation,Cast<AInkCircle>(NowActors[i])->InnerRadius, ExaminationInkActors[i]->GetActorLocation(), Cast<AInkCircle>(ExaminationInkActors[i]) -> InnerRadius , EvaluationValueParameters.MaxShapeDiff),
						0.3,0.3,0.3);
			}

			return CalculateOverallFit(
			CalculateLocationFit(ActorLocation,ExaminationInkActors[i] -> GetActorLocation() , EvaluationValueParameters.MaxDist),
			CalculateRotationFit(ActorRotation,ExaminationInkActors[i] -> GetActorRotation(),EvaluationValueParameters.MaxAngle),
			CalculateScaleFit(ActorScale,ExaminationInkActors[i] -> GetActorScale(),EvaluationValueParameters.MaxScaleDiff),
			0.3f,0.3f,0.3f);
		
			
		};
		float Fit = CalculateFit();
		InkActorFits.Add(Fit);
	}
	float Sum = 0;
	for (float i : InkActorFits)
	{
		Sum += i;
	}
	float AllFit = Sum / InkActorFits.Num();
	if (bLegal)
	{
		TaskSucceed(AllFit);
	}
	else
	{
		TaskFailed(AllFit);
	}
	OnComplete.Broadcast();
}

void AInkTask::TaskFailed(float Fit)
{
	OnFailed.Broadcast(Fit);
}


void AInkTask::TaskSucceed(float Fit)
{
    OnSuccessful.Broadcast(Fit);
}

void AInkTask::AllTasksCompleted()
{
	OnAllTasksComplete.Broadcast();
}

void AInkTask::LoadNextTask()
{
	if (AllTasks.IsValidIndex(NowOperatingWood))
	{
		NowOperatingWood++;
		InkActorFactory->DeleteAllInkActors();
		// if (NowOperatingWood >= AllTasks.Num())
		// {
		// 	AllTasksCompleted();
		// 	return;
		// }
		InkActorFactory->ReceiveThisTask(AllTasks[NowOperatingWood]);
		OnChanged.Broadcast();
	}
	else
	{
		AllTasksCompleted();
	}
}


// Called every frame
void AInkTask::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


