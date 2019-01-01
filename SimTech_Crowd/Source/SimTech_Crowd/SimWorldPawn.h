// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include "Boid.h"
#include "PreyPack.h"
#include "PredatorPack.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "SimWorldPawn.generated.h"

UCLASS()
class SIMTECH_CROWD_API ASimWorldPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASimWorldPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/// @brief initialise simulation objects here
	void initSim();
	/// @brief initialise simulation objects here, callable in blueprint
	/// @param [in] _prey the prey group to spawn
	/// @param [in] _pred the predators to spawn
	UFUNCTION(BlueprintCallable)
	void initSim(UPARAM(ref) APreyPack* &_prey, UPARAM(ref) APredatorPack* &_pred);
	/// @brief whether the world should auto generate agents, edit in editor
	UPROPERTY(BlueprintReadOnly, EditAnyWhere)
	bool m_auto;
	///@brief Radius of the simulation world
	UPROPERTY(EditAnyWhere)
	float m_worldRad;
	///@brief The herd of preys in this simulation
	UPROPERTY(BlueprintReadWrite)
	APreyPack* m_preys;
	///@brief The predator in this simulation
	UPROPERTY(BlueprintReadWrite)
	APredatorPack* m_predators;
	///@brief The bounding sphere
	UPROPERTY()
	USphereComponent* m_bound;

	/// @brief function delegate to be called when an object collides or leaves the world sphere
	/// @param [in] _overlappedComponent the component that was collided, not used here
	/// @param [in] _otherActor the actor that triggered this event
	/// @param [in] _otherComp the component that triggered this event, not used here
	/// @param [in] _otherBodyIndex bodyIndex collided with the sphere, not used here
	UFUNCTION()
	void onBoidLeavingBound(UPrimitiveComponent* _overlappedComponent, AActor*_otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);
};
