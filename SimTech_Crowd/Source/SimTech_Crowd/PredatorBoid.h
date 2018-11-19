// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "PredatorBoid.generated.h"



// Wolf Social Hierarchy
UENUM(BlueprintType)
enum class EWolfSocial : uint8
{
	ALPHA UMETA(DisplayName = "Alpha Male"),
	HIGHER UMETA(DisplayName = "Higher Ranks"),
	LOWER UMETA(DisplayName = "Lower Ranks"),
	OMEGA UMETA(DisplayName = "Lowest Rank"),
	OTHER UMETA(DisplayName = "Other")
};




UCLASS()
class SIMTECH_CROWD_API APredatorBoid : public ABoid
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredatorBoid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void setMesh() override;
	virtual void update() override;
	FVector followLead() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
