// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "PredatorBoid.generated.h"

class APreyBoid;

// Wolf Social Hierarchy
UENUM(BlueprintType)
enum class EWolfSocial : uint8
{
	ALPHA UMETA(DisplayName = "Alpha Male"),
	OMEGA UMETA(DisplayName = "Lowest Rank"),
	OTHER UMETA(DisplayName = "Other")
};

/// predefineclass
class APredatorPack;

UCLASS()
class SIMTECH_CROWD_API APredatorBoid : public ABoid
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredatorBoid();
	UFUNCTION()
	static APredatorBoid* build(UWorld* _w, APredatorPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax);
	UPROPERTY()
	EWolfSocial m_rank;

	virtual void update(const float &_dt) override;
	virtual void handleStatus() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	FVector followLead() const;

	UFUNCTION()
	FVector regroup();
	/// Pointer to the pack this boid belonggs to
	UPROPERTY()
	APredatorPack* m_myPack;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
