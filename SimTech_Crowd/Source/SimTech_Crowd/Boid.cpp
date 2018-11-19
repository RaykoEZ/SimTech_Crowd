// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"


// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	m_mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("BoidMesh"));

	m_invMass = 1 / m_mass;
	m_type = EBoidType::OTHER;
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

