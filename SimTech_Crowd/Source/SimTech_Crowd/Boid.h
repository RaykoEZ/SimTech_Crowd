// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Core/Public/Math/RandomStream.h"
#include "Boid.generated.h"



// Type of boid in scene
UENUM(BlueprintType)
enum class EBoidType : uint8
{
	PREDATOR UMETA(DisplayName = "Predator"),
	PREY UMETA(DisplayName = "Prey"),
	OTHER UMETA(DisplayName = "Other"),

};

// Type of boid in scene
UENUM(BlueprintType)
enum class EBoidStatus : uint8
{
	IDLE UMETA(DisplayName = "Idle"),
	WANDERING UMETA(DisplayName = "Wandering"),
	FLEEING UMETA(DisplayName = "Fleeing"),
	PURSUING UMETA(DisplayName = "Pursuing"),
	REGROUP UMETA(DisplayName = "Regrouping")
};


UCLASS()
class SIMTECH_CROWD_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	/// @brief updates the agent every frame
	/// @param [in] _dt time difference from the previous call
	virtual void update(const float &_dt);
	/// @brief monitors and modifies the states of this agent
	virtual void handleStatus();
	/// @brief function called when an agent enters this neighbourhood
	virtual void onEnterRange();
	/// @brief steers agent towards a target position
	/// @return steering force
	FVector seek() const;
	/// @brief steers agent away from a target position
	/// @return steering force
	FVector flee();
	/// @brief applies force to the agent and updates position
	/// @param [in] _force to use
	void resolve(const FVector &_f);
	/// @brief steer the agent to simulate wandering/grazing
	/// @return random target position to steer towards
	FVector wander()const; /// d
	/// @brief get the average position of the typed agents in the neighbourhood
	/// @param [in] _t type of agent position to look for
	/// @return a target vector to steer the boid towards to approach/leave a neighbourhood
	FVector getAverageNeighbourPos(const EBoidType &_t); ///d

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	///@brief type of boid
	UPROPERTY(BlueprintReadOnly)
	EBoidType m_type;
	///@brief general behaviour that determines the flocking behaviour of the agent
	UPROPERTY(BlueprintReadOnly)
	EBoidStatus m_status;

	/// @brief Search for neighbourhood boid types
	/// @return indexes of said types
	UFUNCTION()
	TArray<int> searchPrey() const;
	/// @brief Search for neighbourhood boid types
	/// @return indexes of said types
	UFUNCTION()
	TArray<int> searchPredator()const;
	///@brief pointers to all agents in the neighbourhood
	UPROPERTY(BlueprintReadOnly)
	TArray<ABoid*> m_neighbours;
	/// @brief getter for predators in the local neighbourhood
	/// @return pointers to said typed agents
	UFUNCTION()
	TArray<ABoid*> getPredator() const;
	/// @brief getter for preys in the local neighbourhood
	/// @return pointers to said typed agents
	UFUNCTION()
	TArray<ABoid*> getPrey() const;
	/// @brief prints the visuals of the agents
	/// @param [in] _c colour of the agent
	void printDebug(const FColor &_c)const;
	/// @brief delegate functions to bind SphereComponent overlap events
	/// @param [in] _overlappedComponent the component that was collided, not used here
	/// @param [in] _otherActor the actor that triggered this event
	/// @param [in] _otherComp the component that triggered this event, not used here
	/// @param [in] _otherBodyIndex bodyIndex collided with the sphere, not used here
	UFUNCTION()
	void onBeginPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult & _sweepResult);
	/// @brief delegate functions to bind SphereComponent overlap events
	/// @param [in] _overlappedComponent the component that was collided, not used here
	/// @param [in] _otherActor the actor that triggered this event
	/// @param [in] _otherComp the component that triggered this event, not used here
	/// @param [in] _otherBodyIndex bodyIndex collided with the sphere, not used here
	UFUNCTION()
	void onEndPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor*_otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// @brief setter for the agent type
	/// @param [in] _pos the position of the particle
	void setType(const EBoidType &_t) { m_type = _t; }
	/// @brief getter for m_type
	/// @return the type of this agent
	EBoidType getType() const { return m_type; }
	/// @brief setter for target position
	/// @param [in] _pos the position of the particle
	UFUNCTION()
	void setTarget(const FVector &_pos) { m_target = _pos; }
	///@brief whether this agent is out of bound from the meta agent or the eorld sphere
	UPROPERTY()
	bool m_isOutOfBound;
	///@brief type of this agent
	UPROPERTY()
	int m_id;
	///@brief for a = f/m, 1/m pre-calculated
	UPROPERTY(BlueprintReadOnly)
	float m_invMass;
	///@brief set mesh for a boid
	UPROPERTY(EditAnywhere)
	float m_mass;

	/// @brief max speed gain
	UPROPERTY(EditAnywhere)
	float m_vMax;
	///@brief max speed gain default
	UPROPERTY()
	float m_vMaxDef;
	///@brief max force gain
	UPROPERTY(EditAnywhere)
	float m_fMax;
	///@brief radius for the neighbourhood
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_collisionRad;
	///@brief position of boid
	UPROPERTY(BlueprintReadWrite)
	FVector m_pos;
	
	///@brief current velocity
	UPROPERTY(BlueprintReadWrite)
	FVector m_v;
	///@brief target position to move to/focus on
	UPROPERTY(BlueprintReadWrite)
	FVector m_target;

	///@brief Mesh for a boid
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* m_mesh;

	///@brief bounding sphere for local neighbourhood
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* m_collision;


	///@brief seeded pseudo random number generator for initialisation
	FRandomStream m_rng;
};
