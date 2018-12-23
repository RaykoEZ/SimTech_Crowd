// Fill out your copyright notice in the Description page of Project Settings.

#include "PredatorPack.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
APredatorPack::APredatorPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_spawnCentre = FVector(-900.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void APredatorPack::BeginPlay()
{
	Super::BeginPlay();
	init();
}

void APredatorPack::init()
{
	m_pack.Reserve(m_numMember);
	auto world = GetWorld();
	for (int i = 0; i < m_numMember; ++i)
	{
		FVector pos = m_spawnCentre + FVector(FMath::RandRange(-m_spawnRad, m_spawnRad), FMath::RandRange(-m_spawnRad, m_spawnRad), 0.0f);
		FVector v = FVector(0.0f, 1.0f, 0.0f);
		//UE_LOG(LogTemp, Warning, TEXT("pos gen : (%f , %f, %f)"), pos.X, pos.Y, pos.Z);
		/// Spawning a boid
		/// Get a boid initialized
		APredatorBoid* predator = APredatorBoid::build(world, this, pos, v, 1.0f, 1.0f);
		/// Set this boid as a template for spawning
		FActorSpawnParameters param = FActorSpawnParameters();
		param.Template = Cast<AActor>(predator);
		/// Now spawn!
		m_pack.Add(world->SpawnActor<APredatorBoid>(pos, FRotator(0.0f), param));
		m_pack[i]->setTarget(FVector(-200.0f, 200.0f, 0.0f));
		UE_LOG(LogTemp, Warning, TEXT("m_pos Prey : (%f , %f, %f)"), m_pack[i]->m_pos.X, m_pack[i]->m_pos.Y, m_pack[i]->m_pos.Z);

	}
}

// Called every frame
void APredatorPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int i = 0; i < m_numMember; ++i)
	{
		m_pack[i]->update(DeltaTime);
	}
}

