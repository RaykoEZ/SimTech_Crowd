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
	UFUNCTION()
	static APreyBoid* build(UWorld* _w,const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax);
	virtual void update(const float &_dt) override;
	virtual void handleStatus() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	FVector evade() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
