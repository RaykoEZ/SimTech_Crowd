// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreyBoid.h"
#include "PreyPack.generated.h"

UCLASS()
class SIMTECH_CROWD_API APreyPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreyPack();
	/// Pack of prey boids
	/// Leader of the pack is boid of index 0
	UPROPERTY(BlueprintReadWrite)
	TArray<APreyBoid*> m_pack;
	
	/// determine where the spawn region is
	UPROPERTY(EditAnywhere)
	FVector m_spawnCentre;
	UPROPERTY(EditAnywhere)
	float m_spawnRad = 200.0f;
	
	UPROPERTY(BlueprintReadWrite)
	int m_numMember = 3;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void init();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
