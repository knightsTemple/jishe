// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "InkActors/InkActor.h"
#include "InkCircle.generated.h"

/**
 * 
 */
UCLASS()
class JISHE_API AInkCircle : public AInkActor
{
	GENERATED_BODY()

public:
	AInkCircle();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UProceduralMeshComponent* ProcMesh;

	UPROPERTY(EditDefaultsOnly, Category="Materials")
	UMaterialInterface* RingMaterial;

	float OuterRadius = 0.0f;
	float InnerRadius = 0.0f;
	float RadiusDelta = 0; // 固定内外半径差
    
	virtual void OnMouseChanging(const FVector& NewLocation) override;


private:
	void GenerateRingMesh();
	
};
