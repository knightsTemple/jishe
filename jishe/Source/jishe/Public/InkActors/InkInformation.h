#pragma once
#include "CoreMinimal.h"  // 确保包含 Unreal Engine 的基础头文件
#include "Wood.h"
#include "InkInformation.generated.h"  // 自动生成的头文件

UENUM(BlueprintType)
enum class EInkActorType : uint8
{
	Line,
	Circle
};

USTRUCT(BlueprintType)
struct FInkDataContext
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EInkActorType InkActorType;//参照墨线的类型
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTransform InkActorTransform;//参照墨线的三维坐标
};

USTRUCT(BlueprintType)
struct FWoodInkData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FInkDataContext> InkActorTransforms;//在这块木头上所有的墨线信息
};

USTRUCT(BlueprintType)
struct FInkDatabaseRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FWoodInkData InkData;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AWood> WoodType;
};