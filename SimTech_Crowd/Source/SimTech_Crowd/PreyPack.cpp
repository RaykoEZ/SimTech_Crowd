// Fill out your copyright notice in the Description page of Project Settings.

#include "PreyPack.h"
#include "PredatorPack.h"

// Sets default values
APreyPack::APreyPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_spawnCentre = FVector(4000.0f,0.0f,0.0f);
	m_numMember = 20;
	m_spawnRad = (float)m_numMember * 500.0f;
	m_packState = EHerdStatus::NERVOUS;
	m_dirty = false;
	m_packPos = m_spawnCentre;
	/// setup boundary
	USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootScene"));

	m_packBound = sphereComponent;
	//m_bound->AttachTo(RootComponent);
	sphereComponent->SetSphereRadius(m_spawnRad);


	m_packBound->OnComponentBeginOverlap.AddDynamic(this, &APreyPack::onBeginPresenceOverlap);
	m_packBound->OnComponentEndOverlap.AddDynamic(this, &APreyPack::onEndPresenceOverlap);

	RootComponent = m_packBound;
	RootComponent->bVisible = true;
	RootComponent->bHiddenInGame = false;

	int32 seed = FMath::Rand();
	m_rng.Initialize(seed);
}

// Called when the game starts or when spawned
void APreyPack::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(m_threatTimer, this, &APreyPack::onThreatTimerUpdate, 1.0f, true);

	init();
	
}

void APreyPack::init()
{	
	m_pack.Reserve(m_numMember);

	auto world = GetWorld();
	for (int i = 0; i < m_numMember; ++i)
	{
		FVector pos = m_spawnCentre + FVector(m_rng.FRandRange(-m_spawnRad, m_spawnRad), m_rng.FRandRange(-m_spawnRad, m_spawnRad), 0.0f);
		FVector v = FVector(0.0f, 0.0f, 0.0f);
		//UE_LOG(LogTemp, Warning, TEXT("pos gen : (%f , %f, %f)"), pos.X, pos.Y, pos.Z);
		/// Spawning a boid
		/// Get a boid initialized
		APreyBoid* prey = APreyBoid::build(world,this,pos, v,FMath::FRandRange(2.4f,3.5f), 1.0f);	
		/// Set this boid as a template for spawning
		FActorSpawnParameters param = FActorSpawnParameters();
		param.Template = Cast<AActor>(prey);
		/// Now spawn!
		m_pack.Add(world->SpawnActor<APreyBoid>(pos, FRotator(0.0f),param));
		m_pack[i]->setTarget(FVector(0.0f, 0.0f, 0.0f));
		//UE_LOG(LogTemp, Warning, TEXT("m_pos Prey : (%f , %f, %f)"), m_pack[i]->m_pos.X, m_pack[i]->m_pos.Y, m_pack[i]->m_pos.Z);
	}
	
}

FVector APreyPack::getPackPos() const
{
	FVector averagePos = FVector(0.0f);
	for (int i = 0; i < m_pack.Num(); ++i)
	{
		averagePos += m_pack[i]->m_pos;
	}
	return averagePos / m_pack.Num();
}


/// called for timer event
void APreyPack::onThreatTimerUpdate()
{

	m_packPos = getPackPos();
	//UE_LOG(LogTemp, Warning, TEXT("Update pack status"));
	if (m_threatSite.Num() > 0)
	{
		updateThreatPos();
	}
	
}




void APreyPack::onBeginPresenceOverlap(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult & _sweepResult)
{
	APredatorBoid* intruder = Cast<APredatorBoid>(_otherActor);
	if (intruder != nullptr)
	{
		updateThreat(intruder);
		if(intruder->m_readyToHunt)
		{
			m_packState = EHerdStatus::PANIC;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Predator entering herd"));
	}

}

void APreyPack::onEndPresenceOverlap(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComp, int32 _otherBodyIndex)
{

}

// Called every frame
void APreyPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//UE_LOG(LogTemp, Warning, TEXT("m_pos Prey : (%f , %f, %f)"), m_packPos.X, m_packPos.Y, m_packPos.Z);

	for (int i = 0; i < m_numMember; ++i)
	{
		m_pack[i]->update(DeltaTime);
	}
	SetActorLocation(m_packPos);
	
}

void APreyPack::updateThreat(APredatorBoid* &_predators)
{

	m_threatSite.Add(_predators);

}

void APreyPack::updateThreatPos()
{
	FVector result = FVector(0.0f);

	for (int i = 0; i < m_threatSite.Num(); ++i)
	{
		result += m_threatSite[i]->m_pos;
		//UE_LOG(LogTemp, Warning, TEXT("m_pos Prey : (%f , %f, %f)"), result.X, result.Y, result.Z);
	}
	result /= m_threatSite.Num();
	//UE_LOG(LogTemp, Warning, TEXT("num %d"), m_threatSite.Num());
	
	for(int i = 0; i<m_pack.Num();++i)
	{
		m_pack[i]->m_threatPos = result;
		m_pack[i]->m_numThreat = m_threatSite.Num();
	}
}

