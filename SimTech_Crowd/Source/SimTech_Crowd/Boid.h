// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
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
	REGROUP UMETA(DisplayName = "Regrouping"),
	DEAD UMETA(DisplayName = "Dead")
};


UCLASS()
class SIMTECH_CROWD_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	/// update per tick
	virtual void update(const float &_dt);
	/// determine and change boid status for this boid
	virtual void handleStatus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector seek() const;
	FVector flee() const;

	/// implement these:
	FVector pursue(const FVector &_futureP) const;

	/// Returns random direction to steer forward
	FVector wander()const; /// d
	/// Returns direction vector facing away from the average neighbourhood
	FVector separate(); ///d
	/// Returns a target vector to steer the boid towards to approach a neighbourhood
	FVector cohesion(const EBoidType &_t); ///d
	FVector alignment(); ///d

	/// type of boid
	UPROPERTY(BlueprintReadOnly)
	EBoidType m_type;

	UPROPERTY(BlueprintReadOnly)
	EBoidStatus m_status;

	/// pointers to boids in fov radius
	UPROPERTY(BlueprintReadOnly)
	TArray<ABoid*> m_neighbours;


	/// Search for neighbourhood boid types and returns indexes of said types
	TArray<int> searchPrey() const;
	TArray<int> searchPredator()const;

	void printDebug(const FColor &_c)const;
	/// when one or more antity enters boid's sphere of detection, 
	/// delegate functions to bind SphereComponent overlap events
	UFUNCTION()
	void onBeginPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult & _sweepResult);
	UFUNCTION()
	void onEndPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor*_otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// set and get type of void for builder
	void setType(const EBoidType &_t) { m_type = _t; }
	EBoidType getType() const { return m_type; }
	UFUNCTION()
	void setTarget(const FVector &_pos) { m_target = _pos; }


	/// for a = f/m, 1/m pre-calculated
	UPROPERTY(BlueprintReadOnly)
	float m_invMass;
	/// set mesh for a boid
	UPROPERTY(EditAnywhere)
	float m_mass = 10.0f;

	/// max speed scalr
	UPROPERTY(EditAnywhere)
	float m_vMax;
	/// max speed scalr default
	UPROPERTY()
	float m_vMaxDef;
	/// max force
	UPROPERTY(EditAnywhere)
	float m_fMax;
	
	/// position of boid
	UPROPERTY(BlueprintReadWrite)
	FVector m_pos;
	
	/// current velocity
	UPROPERTY(BlueprintReadWrite)
	FVector m_v;
	/// target position to move to/focus on
	UPROPERTY(BlueprintReadWrite)
	FVector m_target;

	/// Mesh for a boid
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* m_mesh;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USphereComponent* m_collision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_collisionRad;

	UPROPERTY()
	bool m_isOutOfBound;
};
