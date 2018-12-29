// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "PredatorBoid.generated.h"



UENUM(BlueprintType)
enum class EHuntRole : uint8
{
	FRONT UMETA(DisplayName = "Frontline"),
	LFLANK UMETA(DisplayName = "Left Flank"),
	RFLANK UMETA(DisplayName = "Right Flank")
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
	EHuntRole m_role;
	float m_flankAngle;
	bool m_readyToHunt;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	FVector regroup();
	/// Pointer to the pack this boid belonggs to
	UPROPERTY()
	APredatorPack* m_myPack;
	FVector genericBehaviour(const FVector &_f);
	FVector tacticalBehaviour(const FVector &_f);
	void performRole(const FVector &_target, const FVector &_frontDir);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	static APredatorBoid* build(UWorld* _w, APredatorPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax, const EHuntRole &_role);
	void setFlank();
	virtual void update(const float &_dt) override;
	virtual void handleStatus() override;
	virtual void onEnterRange() override;
	


	
};
