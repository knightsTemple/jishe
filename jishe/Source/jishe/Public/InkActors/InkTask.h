// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InkActor.h"
#include "InkInformation.h"
#include "GameFramework/Actor.h"
#include "InkTask.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllTasksCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskSuccessfulDelegate, float , Rate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskFailedDelegate , float ,Rate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskChangedDelegate);

USTRUCT(BlueprintType)
struct FEvaluationValueParameters
{
	GENERATED_BODY()

	// 位置最大允许差异（单位：厘米）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxDist = 100.0f;

	// 旋转最大允许差异（单位：度）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxAngle = 180.0f;

	// 缩放最大允许差异
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxScaleDiff = 0.5f;

	// 半径最大允许差异
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxRadiusDiff = 5.0f;

	//用于评估两个圆形状差异的最大允许差异值
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxShapeDiff = 10.f;
};
UCLASS()
class JISHE_API AInkTask : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInkTask();

	friend class AInkActorFactory;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UDataTable* DataTable;

	UPROPERTY()
	TArray<AInkActor*> ExaminationInkActors;

	UPROPERTY()
	int AllWoodNum = 0;

	UPROPERTY()
	int NowOperatingWood = 0;

	UPROPERTY()
	AInkActorFactory* InkActorFactory;

	UPROPERTY()
	TArray<FInkDatabaseRow> AllTasks;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FEvaluationValueParameters EvaluationValueParameters;

	UPROPERTY(BlueprintAssignable)
	FTaskCompleteDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
	FAllTasksCompleteDelegate OnAllTasksComplete;

	UPROPERTY(BlueprintAssignable)
	FTaskFailedDelegate OnFailed;

	UPROPERTY(BlueprintAssignable)
	FTaskChangedDelegate OnChanged;

	UPROPERTY(BlueprintAssignable)
	FTaskSuccessfulDelegate OnSuccessful;

	UPROPERTY(BlueprintReadOnly)
	TArray<int> InkActorFits;
	
	UFUNCTION()
	void GiveTask() const;

	UFUNCTION()
	void ReceiveExaminationInkActors(const TArray<AInkActor*>& Array);

	UFUNCTION(BlueprintCallable)
	void TaskCompleted(const TArray<AInkActor*>& NowActors);

	UFUNCTION(BlueprintCallable)
	void TaskFailed(float Fit);
	
	UFUNCTION(BlueprintCallable)
	void TaskSucceed(float Fit);

	UFUNCTION(BlueprintCallable)
	void AllTasksCompleted();

	UFUNCTION(BlueprintCallable)
	void LoadNextTask();
	
	UFUNCTION(BlueprintNativeEvent)
	UDataTable* GetInkActorDataTable();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
