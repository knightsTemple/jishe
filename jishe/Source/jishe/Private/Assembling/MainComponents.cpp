// Fill out your copyright notice in the Description page of Project Settings.


#include "Assembling/MainComponents.h"

#include "Assembling/AssemblingTask.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainComponents::AMainComponents()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	CollisionBoxes = CreateDefaultSubobject<USceneComponent>("CollisionBoxes");
	MainStaticMeshes = CreateDefaultSubobject<USceneComponent>("MainStaticMeshes");
	PartStaticMeshes = CreateDefaultSubobject<USceneComponent>("PartStaticMeshes");
	PrimaryActorTick.bCanEverTick = false;
	MakeBoxesArray();
	MakeEnumMap();
	MakeSuccessMap();
	MakeMeshMap();

	TArray<USceneComponent*> ComponentChildren;
	PartStaticMeshes->GetChildrenComponents(false, ComponentChildren);	
	for (USceneComponent* Mesh : ComponentChildren)
	{
		ChangeComponentLook(Mesh , None);
	}
	AssemblingDataTable = Cast<UDataTable>(FSoftObjectPath(TEXT("/Game/AssemblingActors/DT_Assembling.DT_Assembling")).TryLoad());

	TArray<FAssemblingComponentInfo*> ComponentInfoes;
	AssemblingDataTable -> GetAllRows(TEXT("GetAllRows") , ComponentInfoes);
	for (const auto Pair : EnumMap)
	{
		for (const auto Item : ComponentInfoes)
		{
			if (Pair.Value == Item->ComponentEnum)
			{
				GeneralMap[Pair.Key] = *Item;
				break;
			}
		}
	}
	NowTaskClass = Cast<AAssemblingTask>(UGameplayStatics :: GetActorOfClass(GetWorld() , AAssemblingTask::StaticClass()));
}

// Called when the game starts or when spawned
void AMainComponents::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainComponents::MakeEnumMap_Implementation()
{}

void AMainComponents::MakeMeshMap_Implementation()
{}

void AMainComponents::ReturnOriginLook_Implementation(USceneComponent* MeshComponents)
{}

void AMainComponents::MakeSuccessMap()
{
	for (const USceneComponent* Box : Boxes)
	{
		SuccessMap[Box] = false;
	}
}

bool AMainComponents::CheckIsCompleted()
{
	for (const auto& Pair : SuccessMap)
	{
		if (!Pair.Value)
		{
			return false;		
		}
	}
	return true;
}

void AMainComponents::MakeBoxesArray()
{
	CollisionBoxes->GetChildrenComponents(false,Boxes);
}

void AMainComponents::OnHammer(USceneComponent* Component)
{
	if (WaitingHammer == nullptr)
	{
		return;
	}
	WaitingHammer = nullptr;
	OnHammerOver.Broadcast(Component);
	CompletedOnePart(Component);
	
}

void AMainComponents::OnMouseRelease(const FHitResult& HitResult,const EAssemblingComponentsType EnumType)
{
	UBoxComponent* Component = Cast<UBoxComponent>(HitResult.Component.Get());
	if (Component == nullptr)
	{
		return;
	}
	const auto Result = GeneralMap.Find(Component);
	if (Result == nullptr)
	{
		return;
	}
	if (Result->ComponentEnum != EnumType)
	{
		return;
	}
	if (Result -> bNeedHammer)
	{
		OnCanBeHammer.Broadcast(Cast<USceneComponent>(Component));
		WaitingHammer = Component;
	}
	else 
	{
		CompletedOnePart(Component);
	}
}

void AMainComponents::OnMouseHover(const FHitResult& HitResult, const EAssemblingComponentsType EnumType)
{
	UBoxComponent* Component = Cast<UBoxComponent>(HitResult.Component.Get());
	if (Component == nullptr)
	{
		return;
	}
	const auto Result = GeneralMap.Find(Component);
	if (Result == nullptr)
	{
		return;
	}
	if (Result->ComponentEnum  == EnumType)
	{
		ChangeComponentLook(Component , Green);
	}
	else
	{
		ChangeComponentLook(Component , Red);
	}
}

void AMainComponents::OnMouseStopHover(const FHitResult& HitResult)
{
	UBoxComponent* Component = Cast<UBoxComponent>(HitResult.Component.Get());
	if (Component == nullptr)
	{
		return;
	}
	ChangeComponentLook(Component,None);
}

void AMainComponents::ChangeComponentLook(USceneComponent* Component, EComponentLook LookMethod)
{
	if (LookMethod == None)
	{
		MeshMap[Component]->SetVisibility(false);
		return;
	}
	else if (LookMethod == Origin)
	{
		ReturnOriginLook(Component);
		return;
	}
	UMaterialInstance* MaterialInstance = LoadObject<UMaterialInstance>(nullptr , LookMethod == Red ? TEXT("/Game/AssemblingActors/M_Red_Inst.M_Red_inst") :
		TEXT("/Game/AssemblingActors/M_Green_Inst.M_Green_inst"));
	MeshMap[Component]->SetMaterial(0,MaterialInstance);
}

void AMainComponents::CompletedOnePart(USceneComponent* Component)
{
	if (Component == nullptr) return;
	SuccessMap[Component] = true;
	ChangeComponentLook(Component,Origin);
	if(CheckIsCompleted())
	{
		OnComplete.Broadcast();
		if (NowTaskClass != nullptr)
		{
			NowTaskClass -> OneTaskDone();
		}
	}
}

// Called every frame
void AMainComponents::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


