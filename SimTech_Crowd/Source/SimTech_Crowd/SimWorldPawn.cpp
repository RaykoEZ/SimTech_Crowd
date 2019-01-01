// Fill out your copyright notice in the Description page of Project Settings.

#include "SimWorldPawn.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

// Sets default values
ASimWorldPawn::ASimWorldPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootScene"));
	m_auto = true;
	m_bound = sphereComponent;
	//m_bound->AttachTo(RootComponent);
	m_worldRad = 25000.0f;
	sphereComponent->SetSphereRadius(m_worldRad);

	
	m_bound->OnComponentEndOverlap.AddDynamic(this, &ASimWorldPawn::onBoidLeavingBound);
	

	RootComponent = m_bound;
	RootComponent->bVisible = true;
	RootComponent->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void ASimWorldPawn::BeginPlay()
{
	Super::BeginPlay();
	// if no custom prey packs and predators are assigned, auto generate
	if(m_auto)
	{
		initSim();
	}

	
}

// Called every frame
/// Remember to use timer for simulation-----------------------------------------------
void ASimWorldPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASimWorldPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASimWorldPawn::initSim()
{
	auto world = GetWorld();
	if (world)
	{

		m_preys = world->SpawnActor<APreyPack>(FVector(0.0f), FRotator(0.0f));
		m_predators = world->SpawnActor<APredatorPack>(FVector(0.0f), FRotator(0.0f));

		m_preys->m_worldRad = m_worldRad;
		m_predators->m_worldRad = m_worldRad;
		m_predators->m_targetPack = m_preys;


	}
}

void ASimWorldPawn::initSim( APreyPack * &_prey,  APredatorPack * &_pred)
{
	auto world = GetWorld();
	if (world)
	{

		m_preys = _prey;
		m_predators = _pred;
		if(m_preys != nullptr && m_predators != nullptr)
		{
			m_preys->m_worldRad = m_worldRad;
			m_predators->m_worldRad = m_worldRad;
			m_predators->m_targetPack = m_preys;
		
		}



	}

}

void ASimWorldPawn::onBoidLeavingBound(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComp, int32 _otherBodyIndex)
{
	ABoid* escapee = Cast<ABoid>(_otherActor);
	if (escapee != nullptr)
	{
		escapee->m_isOutOfBound = true;
		escapee->handleStatus();
		//escapee->m_target *= 0.5;
		//UE_LOG(LogTemp, Warning, TEXT("Boid out of bound, guiding back"));
	}
	
}





