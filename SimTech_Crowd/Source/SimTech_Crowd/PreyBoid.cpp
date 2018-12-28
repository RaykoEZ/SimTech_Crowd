// Fill out your copyright notice in the Description page of Project Settings.

#include "PreyBoid.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "PreyPack.h"


// Sets default values
APreyBoid::APreyBoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_type = EBoidType::PREY;
	//UE_LOG(LogTemp, Warning, TEXT("invmass: %f"),m_invMass);
	m_status = EBoidStatus::WANDERING;
	m_collisionRad = 1000.0f;
	m_collision->InitSphereRadius(m_collisionRad);

}

APreyBoid* APreyBoid::build(UWorld* _w, APreyPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax)
{
	APreyBoid* out = NewObject<APreyBoid>();
	out->m_pos = _pos;
	out->m_v = _v;
	out->m_vMax = _vMax;
	out->m_vMaxDef = _vMax;
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
	//UE_LOG(LogTemp, Warning, TEXT("valid ref? %s"), valid ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("valid ref? %f"), m_myPack->m_spawnRad);
}



void APreyBoid::update(const float &_dt)
{
	/// force we are using in different cases
	FVector f;
	//ABoid::update(_dt);
	//printDebug();
	handleStatus();
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
			m_vMax = 0.8f*m_vMaxDef;
			
			//UE_LOG(LogTemp, Warning, TEXT("Wandering!"));

			m_target = wander();
			f = seek();

			break;
		}
		case EBoidStatus::FLEEING:
		{
			//UE_LOG(LogTemp, Warning, TEXT("Flee!"));

			m_vMax = FMath::Clamp(m_vMax*1.05f, 1.0f, 1.5f*m_vMaxDef);
			/// flee from the general direction of the predators
			m_target = m_myPack->m_threatPos;
			f = flee();
			break;
		}
		case EBoidStatus::REGROUP:
		{
			m_target = regroup();
			f = 2.0f * seek();
			//UE_LOG(LogTemp, Warning, TEXT("REGROUP!"));

			break;
		}
		default:
			break;
	}
	resolve(f);
	printDebug(FColor(0.0f, 255.0f, 0.0f));
	

}
/// Changes boid states when neighbourhood updates
void APreyBoid::handleStatus()
{
	/// Predators lost preys
	float dist = FVector::Dist(m_pos, m_myPack->m_pack[0]->m_pos);
	float distO = FVector::Dist(m_pos, FVector(0.0f));
	if (dist > 2.0f * m_myPack->m_spawnRad || distO > m_myPack->m_worldRad)
	{
		m_isOutOfBound = true;
	}
	else
	{
		m_isOutOfBound = false;
	}

	if (m_isOutOfBound && m_status != EBoidStatus::FLEEING)
	{
		m_status = EBoidStatus::REGROUP;

		return;
	}

	
	if( m_myPack->m_packState == EHerdStatus::PANIC)
	{

		m_status = EBoidStatus::FLEEING;
	}
	else 
	{
		m_status = EBoidStatus::WANDERING;
	}

}





FVector APreyBoid::regroup()
{
	// get a random position in leader's direction

	if (m_myPack->m_pack[0] != this)
	{
		FVector bossTarget = m_myPack->m_packPos;
		float r = m_myPack->m_spawnRad;
		m_vMax = FMath::Clamp(m_vMax*1.5f, 1.0f, 5.0f);

		FVector aprxTarget = bossTarget +
			FVector(FMath::RandRange(-r, r),
				FMath::RandRange(-r, r), 0.0f);
		return aprxTarget;
	}
	m_vMax = m_vMaxDef;
	m_v = -m_pos.GetSafeNormal();
	return wander();;
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

