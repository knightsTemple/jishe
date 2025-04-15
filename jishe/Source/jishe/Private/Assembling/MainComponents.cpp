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
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	CollisionBoxes->SetupAttachment(Root);
	MainStaticMeshes->SetupAttachment(Root);
	PartStaticMeshes->SetupAttachment(Root);
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AMainComponents::BeginPlay()
{
	Super::BeginPlay();

	MakeBoxesArray();
	MakeEnumMap();
	MakeSuccessMap();
	MakeMeshMap();
	TArray<FAssemblingComponentInfo*> ComponentInfoes;
	AssemblingDataTable = Cast<UDataTable>(FSoftObjectPath(TEXT("/Game/AssemblingActors/DT_Assembling.DT_Assembling")).TryLoad());
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

	TArray<USceneComponent*> ComponentChildren;
	PartStaticMeshes->GetChildrenComponents(true, ComponentChildren);	
	for (USceneComponent* Mesh : ComponentChildren)
	{
		ChangeComponentLook(Mesh , None);
	}
	

	
	
}

void AMainComponents::MakeEnumMap_Implementation()
{}

void AMainComponents::MakeMeshMap_Implementation()
{}

void AMainComponents::ReturnOriginLook_Implementation(USceneComponent* MeshComponents)
{}

void AMainComponents::MakeSuccessMap()
{
	for ( USceneComponent* Box : Boxes)
	{
		SuccessMap.Add(Box,false);
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

void AMainComponents::OnMouseRelease()
{
	USceneComponent* Component = nullptr;
	for (const auto Pair : SuccessMap)
	{
		if (!Pair.Value)
		{
			Component = Pair.Key;
		}
		
	}
	if (Component == nullptr)
	{
		return;
	}
	CompletedOnePart(Component);
	
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
		Component->SetVisibility(false);
		return;
	}
	if (LookMethod == Origin)
	{
		TArray<USceneComponent*> ComponentsChildren;
		Component->GetChildrenComponents(true,ComponentsChildren);
		ComponentsChildren.Add(Component);
		for (USceneComponent* Child : ComponentsChildren)
		{
			Child->SetVisibility(true);
			ReturnOriginLook(Child);
		}
		
		return;
	}
	Component->SetVisibility(true);
	UMaterialInstance* MaterialInstance = LoadObject<UMaterialInstance>(nullptr , LookMethod == Red ? TEXT("/Game/AssemblingActors/M_Red_Inst.M_Red_inst") :
		TEXT("/Game/AssemblingActors/M_Green_Inst.M_Green_inst"));
	MeshMap[Component]->SetMaterial(0,MaterialInstance);
}

void AMainComponents::CompletedOnePart(USceneComponent* Component)
{
	if (Component == nullptr) return;
	SuccessMap[Component] = true;
	ChangeComponentLook(MeshMap[Component],Origin);
	OnOnePartComplete.Broadcast();
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


