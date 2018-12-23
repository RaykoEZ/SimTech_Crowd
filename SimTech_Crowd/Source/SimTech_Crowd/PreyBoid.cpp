// Fill out your copyright notice in the Description page of Project Settings.

#include "PreyBoid.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PreyPack.h"


// Sets default values
APreyBoid::APreyBoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_type = EBoidType::PREY;
	//UE_LOG(LogTemp, Warning, TEXT("invmass: %f"),m_invMass);
	m_status = EBoidStatus::WANDERING;
}

APreyBoid* APreyBoid::build(UWorld* _w, APreyPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax)
{
	APreyBoid* out = NewObject<APreyBoid>();
	out->m_pos = _pos;
	out->m_v = _v;
	out->m_vMax = _vMax;
	out->m_fMax = _fMax;
	out->m_myPack = _p;
	return out;
}

// Called when the game starts or when spawned
/// Please don't use m_myPack here, it is not initialized at this point in time
void APreyBoid::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(m_pos);
	//UE_LOG(LogTemp, Warning, TEXT("m_pos Prey : (%f , %f, %f)"), m_pos.X, m_pos.Y, m_pos.Z);
	bool valid = IsValid(m_myPack);
	UE_LOG(LogTemp, Warning, TEXT("valid ref? %s"), valid ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("valid ref? %f"), m_myPack->m_spawnRad);
}



void APreyBoid::update(const float &_dt)
{
	/// force we are using in different cases
	FVector f;
	//ABoid::update(_dt);
	//printDebug();
	switch (m_status)
	{
		case EBoidStatus::IDLE:
		{
			f = FVector(0.0f);
			m_v = FVector(0.0f);
			break;
		}	
		case EBoidStatus::WANDERING:
		{
			m_target = wander();
			f = seek();
			break;
		}
		case EBoidStatus::FLEEING:
		{
			f = flee();
			break;
		}
		case EBoidStatus::PURSUING:
		{
			f = seek();
			break;
		}
		case EBoidStatus::REGROUP:
		{

			break;
		}
		case EBoidStatus::ALIGN:
		{

			break;
		}
		case EBoidStatus::DEAD:
		{
			f = FVector(0.0f);
			m_v = FVector(0.0f);
			break;
		}
		default:
			break;
	}
	/// We do our calculations here
	FVector desiredV = m_target - m_pos;
	FVector outV = desiredV.GetSafeNormal();

	FVector accel = f * m_invMass;
	FVector oldV = m_v + accel;
	m_v = ClampVector(oldV, FVector(-m_vMax * 0.5f), FVector(m_vMax));
	m_pos += m_v;
	m_mesh->SetWorldLocation(m_pos);
	RootComponent->SetWorldLocation(m_pos);
	printDebug(FColor(0.0f,255.0f,0.0f));

	

}
/// Changes boid states when neighbourhood updates
void APreyBoid::handleStatus()
{


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

