// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "PredatorBoid.generated.h"


/// @brief For this formation, you either go to the frontlines, left rear or right rear
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
	/// @brief a builder for the predator agent
	/// @param [in] _m mass of the agent
	/// @param [in] _p pointer to the pack/group to which it belongs 
	/// @param [in] _pos current agent position
	/// @param [in] _v current velocity of the agent
	/// @param [in] _vMax maximum velocity increase scaling of agent
	/// @param [in] _fMax maximum force gain of this agent
	/// @param [in] _role the tactical position this agent is assigned to 
	/// @return the pointer to the instantiated agent
	UFUNCTION()
	static APredatorBoid* build(const float &_m, APredatorPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax, const EHuntRole &_role);
	/// @brief sets the position of an agent's flanking position
	void setFlank();
	/// @brief updates the agent every frame
	/// @param [in] _dt time difference from the previous call
	virtual void update(const float &_dt) override;
	/// @brief monitors and modifies the states of this agent
	virtual void handleStatus() override;
	/// @brief function called when an agent enters this neighbourhood
	virtual void onEnterRange() override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/// @brief check if the particle is in the box.
	/// @return a target location near the pack
	UFUNCTION()
	FVector regroup();

	/// @brief when packs atart hunting, we begin to activate flocking patterns to pursue the target
	/// @param [in/out] _f force to be modified
	/// @return modified steering force
	FVector genericBehaviour(const FVector &_f);
	/// @brief When observing, determines which action is performed depending on the role of this agent
	/// @return steering force to move the agent to target location
	FVector tacticalBehaviour();
	/// @brief When observing, agents know their roles and carry out their own positioning
	/// @param [in] _target target position of our pack of prey
	/// @param [in] _frontDir vector from the frontline to the target prey's general position
	void performRole(const FVector &_target, const FVector &_frontDir);

	///@brief Pointer to the pack this boid belonggs to
	UPROPERTY()
	APredatorPack* m_myPack;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	///@brief determins whether this agent is ready to hunt
	bool m_readyToHunt;
	///@brief determines the angles and sides of flanking for this agent
	EHuntRole m_role;
	///@brief the angle this agent should aim to flank
	float m_flankAngle;



	
};
