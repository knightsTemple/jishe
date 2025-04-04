// Fill out your copyright notice in the Description page of Project Settings.


#include "InkActors/InkCircle.h"

AInkCircle::AInkCircle()
{
    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    RootComponent = ProcMesh;

    
}

void AInkCircle::BeginPlay()
{
    Super::BeginPlay();
    ProcMesh->SetMaterial(0, RingMaterial);
    
}


void AInkCircle::OnMouseChanging(const FVector& NewLocation)
{
        RadiusDelta = NecessaryNum; 
       // 实时更新半径
       OuterRadius = FVector::Distance(StartPosition, NewLocation);
       OuterRadius = FMath::Max(OuterRadius, RadiusDelta);
       InnerRadius = OuterRadius - RadiusDelta;  
       GenerateRingMesh();
}

void AInkCircle::GenerateRingMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector2D> UVs;
    const int32 Segments = 64;

    // 顶点生成
    for(int32 i = 0; i <= Segments; ++i)
    {
        const float Angle = 2 * PI * i / Segments;
        
        // 外环顶点
        Vertices.Add(FVector(
            OuterRadius * FMath::Cos(Angle),
            OuterRadius * FMath::Sin(Angle),
            0
        ));
        
        // 内环顶点
        Vertices.Add(FVector(
            InnerRadius * FMath::Cos(Angle),
            InnerRadius * FMath::Sin(Angle),
            0
        ));
    }

    // 三角形生成
    for(int32 i = 0; i < Segments; ++i)
    {
        const int32 StartIdx = 2 * i;
        Triangles.Add(StartIdx);
        Triangles.Add(StartIdx + 1);
        Triangles.Add(StartIdx + 2);

        Triangles.Add(StartIdx + 1);
        Triangles.Add(StartIdx + 3);
        Triangles.Add(StartIdx + 2);
    }

    ProcMesh->CreateMeshSection(0, Vertices, Triangles, {}, UVs, {}, {}, false);
}
