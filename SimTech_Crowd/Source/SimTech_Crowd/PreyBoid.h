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
	/// @brief a builder for the prey agent
	/// @param [in] _m mass of the agent
	/// @param [in] _p pointer to the pack/group to which it belongs 
	/// @param [in] _pos current agent position
	/// @param [in] _v current velocity of the agent
	/// @param [in] _vMax maximum velocity increase scaling of agent
	/// @param [in] _fMax maximum force gain of this agent
	/// @return the pointer to the instantiated agent
	UFUNCTION()
	static APreyBoid* build(const float &_m, APreyPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax);
	/// @brief updates the agent every frame
	/// @param [in] _dt time difference from the previous call
	virtual void update(const float &_dt) override;
	/// @brief monitors and modifies the states of this agent
	virtual void handleStatus() override;
	/// @brief function called when an agent enters this neighbourhood
	virtual void onEnterRange() override;
	/// @brief we begin to activate flocking patterns for this agent
	/// @param [in/out] _f force to be modified
	/// @return modified steering force
	FVector genericBehaviour(const FVector &_f);
	/// @brief called when a predator has reached and attack this agent, affecting fleeing performance
	/// @param [in] _f force/damage of the attack
	void takeDamage(const float &_f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/// @brief check if the particle is in the box.
	/// @return a target location near the pack
	UFUNCTION()
	FVector regroup();
	///@brief whether this agent is hurt by the predator or not
	bool m_isHurt;
	///@brief Pointer to the pack this boid belonggs to
	UPROPERTY()
	APreyPack* m_myPack;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	///@brief number of threats near the herd
	UPROPERTY()
	int m_numThreat;
	///@brief general position of the threats near the pack
	UPROPERTY()
	FVector m_threatPos;

	
};
