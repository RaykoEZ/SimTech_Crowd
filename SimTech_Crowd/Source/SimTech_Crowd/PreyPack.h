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

	UPROPERTY(BlueprintReadWrite)
	TArray<APreyBoid*> m_pack;
	
	UPROPERTY(BlueprintReadWrite)
	APreyBoid* m_leader;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
