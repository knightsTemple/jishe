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

USTRUCT(BlueprintType)
struct FInkDatabaseRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EInkActorType InkActorType;//这一条参照墨线的类型
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTransform InkActorTransform;//这一条参照墨线的三维坐标
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
	AInkActor* NowOperatingInkActor;//现在正在操作的InkActor

	UPROPERTY()
	TSubclassOf<AInkActor> InkLineClass;//线条Actor的类

	UPROPERTY()
	TSubclassOf<AInkActor> InkCircleClass;//圆环Actor的类

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float LineWidth = 100.f;//线条宽度

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float CircleRadiusDelta = 100.f;//圆环线条宽度

	UPROPERTY()
	TArray<FInkDatabaseRow> ExamingActorsData;//检查的数据

	UPROPERTY()
	TArray<AInkActor*> InkActorsData;//实际上生成的Ink Actor

	UPROPERTY()
	UDataTable* InkDataTable;//存储InkActor数据的数据表

	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DeleteAllInkActors();

	UFUNCTION(BlueprintCallable)
	void DeleteLastActor();

	UFUNCTION(BlueprintCallable)
	void OnMouseChanging(const FVector& NewLocation) ;

	UFUNCTION(BlueprintCallable)
	void OnMouseLeftClick(EInkActorType InkActorType , const FVector& Location);
};

