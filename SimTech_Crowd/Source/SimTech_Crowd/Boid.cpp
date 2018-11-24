// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"


// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_invMass = 1 / m_mass;
	m_type = EBoidType::OTHER;
	
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	//SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	
	//m_mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("BoidMesh"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/Mesh/Blob.Blob'"));
	// Create and position a mesh component so we can see where our sphere is
	//m_mesh->SetupAttachment(RootComponent);
	//m_mesh->SetStaticMesh(mesh.Object);
	//m_mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//m_mesh->SetWorldScale3D(FVector(1.0f));
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Boid Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	m_mesh->SetStaticMesh(MeshAsset.Object);

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}



void ABoid::update()
{
}

void ABoid::updateNeighbour()
{


}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------------------


/// Implementations are based on this paper :
/// Steering Behaviors For Autonomous Characters
/// by Craig W.Reynolds, presented on GDC1999

void ABoid::getTarget()
{
	return;
}

/// Seek a position to steer towards
FVector ABoid::seek() const
{
	FVector desiredV = m_pos - m_target;
	
	FVector outV = desiredV.GetSafeNormal();
	
	outV *= m_vMax;
	outV -= m_v;

	return outV;
}

FVector ABoid::flee() const
{
	/// steer away from the seeking position
	return -seek();
}

FVector ABoid::pursue(const FVector &_futureP) const
{
	FVector desiredV = m_pos - _futureP;

	FVector outV = desiredV.GetSafeNormal();

	outV *= m_vMax;
	outV -= m_v;
	return outV;
}

FVector ABoid::wander() const
{
	return FVector();
}

FVector ABoid::separate() const
{
	return FVector();
}

FVector ABoid::cohesion() const
{
	return FVector();
}

FVector ABoid::alignment() const
{
	return FVector();
}

FVector ABoid::avoidCollision() const
{
	return FVector();
}



