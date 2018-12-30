// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PreyBoid.generated.h"

class APreyPack;

UCLASS()
class SIMTECH_CROWD_API APreyBoid : public ABoid
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreyBoid();
	UFUNCTION()
	static APreyBoid* build(UWorld* _w,APreyPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax);
	virtual void update(const float &_dt) override;
	virtual void handleStatus() override;
	virtual void onEnterRange() override;
	FVector genericBehaviour(const FVector &_f);
	void takeDamage(const float &_f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	FVector regroup();

	/// Pointer to the pack this boid belonggs to
	UPROPERTY()
	APreyPack* m_myPack;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
};
