#pragma once
#include "CoreMinimal.h"  // 确保包含 Unreal Engine 的基础头文件

#include "AssemblingInformation.generated.h"

class AMainComponents;
class APartComponents;

UENUM(BlueprintType)
enum class EAssemblingComponentsType : uint8
{
	Type1,
	Type2,
	Type3,
	Type4,
	Type5,
	Type6,
	Type7,
	Type8,
	Type9,
	Type10,
	Type11,
	Type12,
	Type13,
	Type14,
	Type15
};


USTRUCT(BlueprintType)
struct FAssemblingComponentInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAssemblingComponentsType ComponentEnum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedHammer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APartComponents> ComponentClass;
};

UENUM(BlueprintType)
enum EComponentLook
{
	Red,//表示错误零件
	Green,//表示正确零件
	Origin,//表示零件本来的材质
	None//表示不显示零件
};

USTRUCT()
struct FAssemblingTasks : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMainComponents> ComponentClass;
};
