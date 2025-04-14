#pragma once

#include "CoreMinimal.h"
#include "AssemblingInformation.h"

#include "AssemblingTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinished);

UCLASS()
class JISHE_API AAssemblingTask : public AActor
{
	GENERATED_BODY()
	friend class AMainComponents;
public:
	AAssemblingTask();
	
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	int NowTask = 0;

	UPROPERTY(BlueprintReadOnly)
	AMainComponents* NowMainComponents;

	UPROPERTY()
	TArray<FAssemblingTasks> NowTasks;

	UPROPERTY()
	UDataTable* NowDataTable;

	UPROPERTY(BlueprintReadWrite , EditAnywhere)
	FVector SpawnLocation = {0,0,0};

	UPROPERTY(BlueprintAssignable)
	FOnChanged OnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnFinished OnFinished;
	
	UFUNCTION()
	void AllDone();

	UFUNCTION()
	void OneTaskDone();

	
};
