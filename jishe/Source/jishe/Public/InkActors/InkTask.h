// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InkActor.h"
#include "InkInformation.h"
#include "GameFramework/Actor.h"
#include "InkTask.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllTasksCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskSuccessfulDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskFailedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskChangedDelegate);

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
	void TaskFailed();
	
	UFUNCTION(BlueprintCallable)
	void TaskSucceed();

	UFUNCTION(BlueprintCallable)
	void AllTasksCompleted();

	UFUNCTION(BlueprintCallable)
	void TaskChanges();
	
	UFUNCTION(BlueprintNativeEvent)
	UDataTable* GetInkActorDataTable();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
