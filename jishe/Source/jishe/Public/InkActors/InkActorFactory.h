// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InkInformation.h"
#include "InkActorFactory.generated.h"

class AWood;
class AInkActor;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorNumChanged , float , NumRate);
UCLASS()
class JISHE_API AInkActorFactory : public AActor
{
	GENERATED_BODY()


	
public:
	// Sets default values for this actor's properties
	AInkActorFactory();

	friend class AInkTask;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AInkActor*> AllInkActors;//目前创建过的InkActor

	UPROPERTY()
	AInkActor* NowOperatingInkActor;//现在正在操作的InkActor

	UPROPERTY()
	TSubclassOf<AInkActor> InkLineClass;//线条Actor的类

	UPROPERTY()
	TSubclassOf<AInkActor> InkCircleClass;//圆环Actor的类

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float LineWidth = 100.f;//线条宽度

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float CircleRadiusDelta = 100.f;//圆环线条宽度

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector WoodSpawnLocation = {0,0,0};

	UPROPERTY()
	TArray<AInkActor*> InkActorsData;//实际上生成的Ink Actor
	
	UPROPERTY()
	AWood* NowWood = nullptr;

	UPROPERTY()
	AInkTask* NowInkTaskClass;

	UPROPERTY()
	int NowLineNum = 0;

	UPROPERTY()
	int MaxActorNum = 0;

	UPROPERTY()
	FInkDatabaseRow NowTask;

	UFUNCTION()
	void ReceiveThisTask(FInkDatabaseRow& ThisTask);
	
	UFUNCTION()
	void GenerateExaminationInkActors();


		
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnActorNumChanged OnActorNumChanged;

	UFUNCTION(BlueprintCallable)
	void DeleteAllInkActors();

	UFUNCTION(BlueprintCallable)
	void DeleteLastActor();

	UFUNCTION(BlueprintCallable)
	void OnMouseChanging(const FVector& NewLocation) ;

	UFUNCTION(BlueprintCallable)
	void OnMouseLeftClick(EInkActorType InkActorType , const FVector& Location);
};

