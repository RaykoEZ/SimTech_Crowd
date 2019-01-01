// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreyBoid.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "PreyPack.generated.h"


class APredatorPack;
// State of the prey herd
UENUM(BlueprintType)
enum class EHerdStatus : uint8
{
	NERVOUS UMETA(DisplayName = "Nervous"),
	PANIC UMETA(DisplayName = "Panic")

};


UCLASS()
class SIMTECH_CROWD_API APreyPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreyPack();

	///@brief number of agents in this group at the start
	UPROPERTY(EditAnywhere)
	int m_numMember;
	///@brief Meta-state of the agents, affects flocking behaviour
	EHerdStatus m_packState;
	///@brief radius of the agent spawn area
	UPROPERTY(EditAnywhere)
	float m_spawnRad;
	///@brief reference of the world radius
	UPROPERTY()
	float m_worldRad;
	///@brief centre position of the spawning cricle
	UPROPERTY(EditAnywhere)
	FVector m_spawnCentre;

	///@brief the average position of the prey pack
	UPROPERTY()
	FVector m_packPos;
	///@brief Pack of prey boids
	/// Leader of the pack is boid of index 0
	UPROPERTY(BlueprintReadWrite)
	TArray<APreyBoid*> m_pack;
	///@brief latest sighting of predators
	UPROPERTY()
	TArray<APredatorBoid*> m_threatSite;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/// @brief initialise meta agent content
	void init();
	/// @brief updates average position of the agents in the group
	/// @return new average position
	FVector getPackPos() const;

	
	///@brief bounding sphere of the meta agent for this herd of preys
	UPROPERTY()
	USphereComponent* m_packBound;
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

	/// @brief update predator sighting
	/// @param [in] _predators
	UFUNCTION()
	void updateThreat(APredatorBoid* &_predators);
	/// @brief update predators' general position
	UFUNCTION()
	void updateThreatPos();
	/// @brief timer functon to be called every time step
	UFUNCTION()
	void onThreatTimerUpdate();
	///@brief a timer to update our pack data per time step
	FTimerHandle m_threatTimer;
	///@brief seeded pseudo random number generator for initialisation
	FRandomStream m_rng;
};
