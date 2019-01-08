// Fill out your copyright notice in the Description page of Project Settings.

#include "PredatorPack.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
APredatorPack::APredatorPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_spawnCentre = FVector(-20000.0f, 0.0f, 0.0f);
	m_numMember = 7;
	m_spawnRad = m_numMember * 500.0f;
	m_huntStatus = EPackStatus::OBSERVING;
	
	/// setup boundary
	USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootScene"));

	m_packBound = sphereComponent;
	sphereComponent->SetSphereRadius(m_spawnRad);

	RootComponent = m_packBound;
	RootComponent->bVisible = true;
	RootComponent->bHiddenInGame = false;

	int32 seed = FMath::Rand();
	m_rng.Initialize(seed);
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
	for (uint8 i = 0; i < m_numMember; ++i)
	{
		FVector pos = m_spawnCentre + FVector(m_rng.FRandRange(-m_spawnRad, m_spawnRad), m_rng.FRandRange(-m_spawnRad, m_spawnRad), 0.0f);
		FVector v = FVector(1.0f, 0.3f, 0.0f);
		//UE_LOG(LogTemp, Warning, TEXT("pos gen : (%f , %f, %f)"), pos.X, pos.Y, pos.Z);
		EHuntRole role = assignRole(i);	
		/// Spawning a boid

		/// Get a boid initialized
		APredatorBoid* predator = APredatorBoid::build(10.0f, this, pos, v, FMath::FRandRange(3.5f, 5.0f), 1.0f, role);
		predator->m_id = (int)i;

		/// Set this boid as a template for spawning
		FActorSpawnParameters param = FActorSpawnParameters();
		param.Template = Cast<AActor>(predator);
		/// Now spawn!
		m_pack.Add(world->SpawnActor<APredatorBoid>(pos, FRotator(0.0f), param));
		m_pack[i]->m_role = assignRole(i);
		m_pack[i]->setFlank();
		//UE_LOG(LogTemp, Warning, TEXT("FORMATION! Role: %d"), (int)m_pack[i]->m_role);

		m_pack[i]->setTarget(FVector(-200.0f, 200.0f, 0.0f));
		//UE_LOG(LogTemp, Warning, TEXT("m_pos Prey : (%f , %f, %f)"), m_pack[i]->m_pos.X, m_pack[i]->m_pos.Y, m_pack[i]->m_pos.Z);

	}
	
}

EHuntRole APredatorPack::assignRole(const uint8 & _i)
{
	uint8 r = (uint8)_i % 3;
	return EHuntRole(r);
}

APreyBoid* APredatorPack::getDesiredPrey() const
{
	APreyBoid* out = m_targetPack->m_pack[0];
	float v = m_targetPack->m_pack[0]->m_vMaxDef;

	for(int i=1;i<m_targetPack->m_pack.Num();++i)
	{
		if(m_targetPack->m_pack[i]->m_vMaxDef < v)
		{
			v = m_targetPack->m_pack[i]->m_vMaxDef;
			out = m_targetPack->m_pack[i];
		}
	}
	return out;
}



// Called every frame
void APredatorPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	for (int i = 0; i < m_numMember; ++i)
	{
		m_pack[i]->update(DeltaTime);
		
	}
	if(m_targetPack->m_packState == EHerdStatus::PANIC && m_huntStatus == EPackStatus::OBSERVING)
	{
		m_desiredPrey = getDesiredPrey();
		m_huntStatus = EPackStatus::HUNTING;
		
	}
	SetActorLocation(m_pack[0]->m_pos);
}

