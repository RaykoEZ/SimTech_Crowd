// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Core/Public/Templates/UniquePtr.h"
#include "Boid.generated.h"



// Type of boid in scene
UENUM(BlueprintType)
enum class EBoidType : uint8
{
	PREDATOR UMETA(DisplayName = "Predator"),
	PREY UMETA(DisplayName = "Prey"),
	OTHER UMETA(DisplayName = "Other"),

};

UCLASS()
class SIMTECH_CROWD_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// update per tick
	virtual void update();
	
	/// update neighbourhood
	void updateNeighbour();

	FVector seek() const;
	FVector flee() const;
	FVector pursue(const FVector &_futureP) const;
	FVector wander() const;
	FVector separate() const;
	FVector cohesion() const;
	FVector alignment() const;
	FVector avoidCollision() const;

	/// type of boid
	EBoidType m_type;
	/// pointers to boids in fov radius
	UPROPERTY(BlueprintReadOnly)
	TArray<ABoid*> m_neigbours;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Mesh for a boid
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent *m_mesh;

	/// set and get type of void for builder
	void setType(const EBoidType &_t) { m_type = _t; }
	EBoidType getType() const { return m_type; }

	/// for a = f/m, 1/m pre-calculated
	float m_invMass = 1.0f;
	/// set mesh for a boid
	UPROPERTY(EditAnywhere)
	float m_mass = 10.0f;

	/// field of vision of boid, for neighbourhood
	UPROPERTY(EditAnywhere)
	float m_fov = 60.0f;

	/// radius of vision field for nieghbourhood
	UPROPERTY(EditAnywhere)
	float m_vRad = 1.0f;

	/// max speed scalr
	UPROPERTY(EditAnywhere)
	float m_vMax;

	/// max force
	UPROPERTY(EditAnywhere)
	float m_fMax;

	/// Orientation of boid
	UPROPERTY(BlueprintReadWrite)
	FRotator m_facing;
	
	/// position of boid
	UPROPERTY(BlueprintReadWrite)
	FVector m_pos;
	
	/// current velocity
	UPROPERTY(BlueprintReadWrite)
	FVector m_v;

	/// target position to move to/focus on
	UPROPERTY(BlueprintReadWrite)
	FVector m_target;
	

};
