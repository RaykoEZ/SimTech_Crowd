// Fill out your copyright notice in the Description page of Project Settings.

#include "PreyBoid.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
APreyBoid::APreyBoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_type = EBoidType::PREY;
	//UE_LOG(LogTemp, Warning, TEXT("invmass: %f"),m_invMass);
}

APreyBoid* APreyBoid::build(UWorld* _w, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax)
{
	
	APreyBoid* out = _w->SpawnActor<APreyBoid>(_pos, FRotator(0.0f));
	out->m_pos = _pos;
	out->m_v = _v;
	out->m_vMax = _vMax;
	out->m_fMax = _fMax;
	return out;
}

// Called when the game starts or when spawned
void APreyBoid::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(m_pos);
	//UE_LOG(LogTemp, Warning, TEXT("m_pos Prey : (%f , %f, %f)"), m_pos.X, m_pos.Y, m_pos.Z);

}



void APreyBoid::update(const float &_dt)
{
	ABoid::update(_dt);
	//printDebug();
}

FVector APreyBoid::evade() const
{
	return FVector();
}

// Called every frame
void APreyBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

