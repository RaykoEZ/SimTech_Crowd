// Fill out your copyright notice in the Description page of Project Settings.

#include "PredatorBoid.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PredatorPack.h"


// Sets default values
APredatorBoid::APredatorBoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_type = EBoidType::PREDATOR;
	m_status = EBoidStatus::WANDERING;
	m_readyToHunt = false;
	m_collisionRad = 500.0f;
	m_collision->InitSphereRadius(m_collisionRad);

}

APredatorBoid* APredatorBoid::build(UWorld* _w, APredatorPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax)
{
	APredatorBoid* out = NewObject<APredatorBoid>();
	out->m_pos = _pos;
	out->m_v = _v;
	out->m_vMax = _vMax;
	out->m_vMaxDef = _vMax;
	out->m_fMax = _fMax;
	out->m_myPack = _p;

	return out;
}

// Called when the game starts or when spawned
void APredatorBoid::BeginPlay()
{
	Super::BeginPlay();


}



void APredatorBoid::update(const float &_dt)
{
	/// force we are using in different cases
	/// do things according to your thing
	
	FVector f;
	if(m_myPack->m_huntStatus==EPackStatus::HUNTING)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HUNT!"));

		handleStatus();
		f = genericBehaviour(f);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("TACTICS!"));

		f = tacticalBehaviour(f);
	}

	resolve(f);
	printDebug(FColor(255.0f, 0.0, 0.0f));
}


void APredatorBoid::handleStatus()
{
	
	/// Predators lost preys
	float dist = FVector::Dist(m_pos, m_myPack->m_pack[0]->m_pos);
	float distO = FVector::Dist(m_pos, FVector(0.0f));
	if (m_myPack->m_desiredPrey != nullptr)
	{
		
		m_status = EBoidStatus::PURSUING;
		//UE_LOG(LogTemp, Warning, TEXT("Found prey!"));
		//m_vMax *= 1.15f;
		return;
	}

	if (dist > m_myPack->m_spawnRad || distO > m_myPack->m_worldRad)
	{
		m_isOutOfBound = true;
	}
	else
	{
		m_isOutOfBound = false;
	}

	if (m_isOutOfBound)
	{
		m_status = EBoidStatus::REGROUP;
		//m_isOutOfBound = false;
		//UE_LOG(LogTemp, Warning, TEXT("Out of hunting ground!"));

		return;
	}
	else
	{
		m_status = EBoidStatus::WANDERING;
		//UE_LOG(LogTemp, Warning, TEXT("Wandering!"));

	}
}



FVector APredatorBoid::regroup()
{
	// get a random position in leader's direction

	if (m_id != 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("REGROUP!"));

		FVector bossTarget = m_myPack->m_pack[0]->wander();
		float r = m_myPack->m_spawnRad;
		m_vMax = FMath::Clamp(m_vMax*1.5f, 1.0f, 5.0f);

		FVector aprxTarget = bossTarget +
			FVector(FMath::RandRange(-r, r),
				FMath::RandRange(-r, r), 0.0f);
		return aprxTarget;
	}
	m_vMax = m_vMaxDef;
	m_v =   -m_pos.GetSafeNormal();
	return wander();
}

FVector APredatorBoid::genericBehaviour(const FVector &_f)
{
	FVector f = _f;
	switch (m_status)
	{
	case EBoidStatus::IDLE:
	{
		f = FVector(0.0f);
		m_v = FVector(0.0f);
		m_target = m_pos;
		//UE_LOG(LogTemp, Warning, TEXT("Idle!"));

		break;
	}
	case EBoidStatus::WANDERING:
	{
		m_vMax = m_vMaxDef;
		m_target = wander();
		f = seek();
		//UE_LOG(LogTemp, Warning, TEXT("Wandering!"));


		break;
	}
	case EBoidStatus::PURSUING:
	{
		m_vMax = FMath::Clamp(m_vMax*1.2f, 1.0f, 2.0f*m_vMaxDef);
		m_target = m_myPack->m_desiredPrey->wander();
		f = seek();
		//UE_LOG(LogTemp, Warning, TEXT("Pursue!"));

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
	return f;
}

FVector APredatorBoid::tacticalBehaviour(const FVector & _f)
{
	//m_vMax = 3.0f*m_vMaxDef;
	FVector targetP;
	//GetWorld()->GetTimerManager().PauseTimer(m_myPack->m_targetPack->m_threatTimer);
	targetP = m_myPack->m_targetPack->m_packPos;
	//GetWorld()->GetTimerManager().UnPauseTimer(m_myPack->m_targetPack->m_threatTimer);

	float dist = FVector::Dist(m_pos, targetP);
	bool isFar = dist - m_myPack->m_spawnRad - m_myPack->m_targetPack->m_spawnRad >=  m_collisionRad;
	if(isFar)
	{
		m_v = m_myPack->m_targetPack->m_packPos - m_pos;
		m_target = wander();
	}
	else if(!isFar && !m_readyToHunt)
	{
		//UE_LOG(LogTemp, Warning, TEXT("FORMATION!"));

		performRole(targetP);
	}
	
	
	return seek();
}

void APredatorBoid::performRole(const FVector &_target)
{
	FVector front = m_myPack->m_pack[0]->m_pos - _target;
	FVector dir;
	FVector flankPos;
	
	//UE_LOG(LogTemp, Warning, TEXT("FORMATION! Role: %d"), (int)m_role);
	switch (m_role)
	{
	case EHuntRole::FRONT:
	{
		m_vMax = 0.7f*m_vMaxDef;
		flankPos = _target;
		m_readyToHunt = true;
		break;
	}
	case EHuntRole::LFLANK:
	{
		flankPos = _target + front * FRotator(0.0f, -90.0f, 0.0f).Vector();
		
		dir = front.RotateAngleAxis(-90.0f, FVector(0.0f, 0.0f, 1.0f));
		//flankPos = _target + m_myPack->m_targetPack->m_spawnRad * dir;
		m_vMax = 1.5f*m_vMaxDef;
		break;
	}
	case EHuntRole::RFLANK:
	{
		flankPos = _target + front * FRotator(0.0f, 90.0f, 0.0f).Vector();

		//UE_LOG(LogTemp, Warning, TEXT("getting desired prey"));
		dir = front.RotateAngleAxis(90.0f, FVector(0.0f, 0.0f, 1.0f));
		//flankPos = _target + m_myPack->m_targetPack->m_spawnRad * dir;
		m_vMax = 1.5f*m_vMaxDef;
		break;
	}
	default:
		break;
	}

	m_v = dir - front;
	m_target = flankPos;


}



// Called every frame
void APredatorBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

