// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PredatorBoid.h"
#include "PredatorPack.generated.h"

UCLASS()
class SIMTECH_CROWD_API APredatorPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredatorPack();
	
	UPROPERTY(BlueprintReadWrite)
	TArray<APredatorBoid*> m_pack;
	
	UPROPERTY(BlueprintReadWrite)
	APredatorBoid* m_leader;
	
	UPROPERTY(BlueprintReadWrite)
	int m_numMember = 3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
