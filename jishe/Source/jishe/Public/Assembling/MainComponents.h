// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assembling/AssemblingInformation.h"
#include "MainComponents.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCanBeHammer, USceneComponent* , Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHammerOver,USceneComponent*,Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComplete);


UCLASS()
class JISHE_API AMainComponents : public AActor
{
	GENERATED_BODY()

	friend  class AAssemblingTask;

public:
	// Sets default values for this character's properties
	AMainComponents();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	USceneComponent* CollisionBoxes;//下放处理好的碰撞体（对线型检测敏感

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	USceneComponent* PartStaticMeshes;//下放附加的需要拼接的静态网格体

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	USceneComponent* MainStaticMeshes;//下放主要的不用拼接的静态网格体

	//使用哈希表来精确到碰撞体和需要的枚举
	UPROPERTY(BlueprintReadWrite,Category="Components")
	TMap<USceneComponent*,EAssemblingComponentsType>  EnumMap;

	UPROPERTY(BlueprintReadOnly,Category="Components")
	TMap<USceneComponent*,bool>  SuccessMap;//判断所有是否完工
	
	//盒型碰撞和所对应的静态网格体
	UPROPERTY(BlueprintReadWrite,Category="Components")
	TMap<USceneComponent*,UStaticMeshComponent*>  MeshMap;

	UPROPERTY()
	TMap<USceneComponent* , FAssemblingComponentInfo> GeneralMap;//结构体的哈希表
	
	UPROPERTY()
	TArray<USceneComponent*> Boxes;//使用到的碰撞箱

	UPROPERTY()
	USceneComponent* WaitingHammer;//等待被锤

	UPROPERTY()
	UDataTable* AssemblingDataTable;//使用到的数据表格

	UPROPERTY(BlueprintAssignable)
	FOnCanBeHammer	OnCanBeHammer;

	UPROPERTY(BlueprintAssignable)
	FOnHammerOver	OnHammerOver;

	UPROPERTY(BlueprintAssignable)
	FOnComplete		OnComplete;

	UPROPERTY()
	AAssemblingTask* NowTaskClass;
	
	UFUNCTION(BlueprintNativeEvent)
	void MakeEnumMap();

	UFUNCTION(BlueprintNativeEvent)
	void MakeMeshMap();

	UFUNCTION(BlueprintNativeEvent)
	void ReturnOriginLook(USceneComponent* MeshComponents);

	UFUNCTION()
	void MakeSuccessMap();

	UFUNCTION()
	bool CheckIsCompleted(); 

	UFUNCTION()
	void MakeBoxesArray();

	UFUNCTION(BlueprintCallable)
	void OnHammer(USceneComponent* Component);

	UFUNCTION(BlueprintCallable)
	void OnMouseRelease(const FHitResult& HitResult ,const EAssemblingComponentsType EnumType);

	UFUNCTION(BlueprintCallable)
	void OnMouseHover(const FHitResult& HitResult ,const EAssemblingComponentsType EnumType);

	UFUNCTION(BlueprintCallable)
	void OnMouseStopHover(const FHitResult& HitResult);
	
	UFUNCTION()
	void ChangeComponentLook(USceneComponent* Component , EComponentLook LookMethod);

	UFUNCTION()
	void CompletedOnePart(USceneComponent* Component);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
