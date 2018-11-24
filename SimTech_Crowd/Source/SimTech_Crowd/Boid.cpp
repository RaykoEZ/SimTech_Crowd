// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

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
	
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoid::setMesh()
{
}

void ABoid::update()
{
}

void ABoid::updateNeighbour()
{
}


//------------------------------------------------------------------------

FVector ABoid::getTarget()
{
	return FVector();
}

FVector ABoid::seek() const
{
	return FVector();
}

FVector ABoid::flee() const
{
	return FVector();
}

FVector ABoid::pursue() const
{
	return FVector();
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

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

