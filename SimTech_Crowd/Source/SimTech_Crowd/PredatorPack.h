// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PredatorBoid.h"
#include "PreyPack.h"
#include "PredatorPack.generated.h"

/// Stages of the hunt
UENUM(BlueprintType)
enum class EPackStatus : uint8
{
	OBSERVING UMETA(DisplayName = "Observing"),
	HUNTING UMETA(DisplayName = "Hunting")

};



UCLASS()
class SIMTECH_CROWD_API APredatorPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredatorPack();
	///@brief number of agents in this group at the start
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int m_numMember;
	///@brief  Meta-state of the agents, affects flocking behaviour
	UPROPERTY()
	EPackStatus m_huntStatus;
	///@brief radius of the agent spawn area
	UPROPERTY(EditAnywhere)
	float m_spawnRad;
	///@brief reference of the world radius
	UPROPERTY()
	float m_worldRad;

	///@brief centre position of the spawning cricle
	UPROPERTY(EditAnywhere)
	FVector m_spawnCentre;
	///@brief pointer to the targeted herd of preys
	UPROPERTY()
	APreyPack* m_targetPack;
	///@brief the chosen prey to chase after
	APreyBoid* m_desiredPrey;
	///@brief Pack of prey boids
	/// Leader of the pack is boid of index 0
	UPROPERTY(BlueprintReadWrite)
	TArray<APredatorBoid*> m_pack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/// @brief initialise meta agent content
	void init();
	/// @brief check if the particle is in the box.
	/// @param [in] _pos the position of the particle
	/// @return true for in box, false for not in box
	EHuntRole assignRole(const uint8 &_i);
	/// @brief check if the particle is in the box.
	/// @return true for in box, false for not in box
	APreyBoid* getDesiredPrey() const;
	///@brief bounding sphere of the meta agent for this pack of predators, decisions were to not implement additional functions for the pack yet
	UPROPERTY()
	USphereComponent* m_packBound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	///@brief seeded pseudo random number generator for initialisation
	FRandomStream m_rng;
	
};
