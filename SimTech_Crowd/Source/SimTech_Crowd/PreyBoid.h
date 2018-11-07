// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "PreyBoid.generated.h"

UCLASS()
class SIMTECH_CROWD_API APreyBoid : public ABoid
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreyBoid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void setMesh() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
