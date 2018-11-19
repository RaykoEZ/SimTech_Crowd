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
	/// Mesh for a boid
	UPROPERTY(BlueprintReadWrite)
	UStaticMeshComponent *m_mesh;

	/// for a = f/m, 1/m pre-calculated
	float m_invMass = 1.0f;
	virtual void setMesh();

	virtual void update();
	
	/// update neighbourhood
	void updateNeighbour();

	FVector seek() const;
	FVector flee() const;
	FVector pursue() const;
	FVector wander() const;
	FVector separate() const;
	FVector cohesion() const;
	FVector alignment() const;
	FVector avoidCollision() const;

	/// type of boid
	EBoidType m_type;
	/// pointers to boids in fov radius
	UPROPERTY(BlueprintReadOnly)
	TArray<ABoid*> m_niegbours;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/// Orientation of boid
	UPROPERTY(BlueprintReadWrite)
	FRotator m_facing;
	
	/// position of boid
	UPROPERTY(BlueprintReadWrite)
	FVector m_pos;
	
	/// current velocity
	UPROPERTY(BlueprintReadWrite)
	FVector m_v;

	/// max velocity
	UPROPERTY(BlueprintReadWrite)
	FVector m_vMax;
	
	/// max force
	UPROPERTY(BlueprintReadWrite)
	FVector m_fMax;

	/// set mesh for a boid
	UPROPERTY(BlueprintReadWrite)
	float m_mass = 1.0f;
	
	/// field of vision of boid, for neighbourhood
	UPROPERTY(BlueprintReadWrite)
	float m_fov = 60.0f;

	/// radius of vision field for nieghbourhood
	UPROPERTY(BlueprintReadWrite)
	float m_vRad = 1.0f;

};
