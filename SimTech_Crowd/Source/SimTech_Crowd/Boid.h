// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Core/Public/Templates/UniquePtr.h"
#include "Boid.generated.h"


UCLASS()
class SIMTECH_CROWD_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/// Mesh for a boid
	TUniquePtr<UStaticMesh> m_mesh;
	/// set mesh for a boid
	virtual void setMesh();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
