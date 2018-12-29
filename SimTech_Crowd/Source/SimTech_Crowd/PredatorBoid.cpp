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

APredatorBoid* APredatorBoid::build(UWorld* _w, APredatorPack* _p, const FVector &_pos, const FVector &_v, const float &_vMax, const float &_fMax, const EHuntRole &_role)
{
	APredatorBoid* out = NewObject<APredatorBoid>();
	out->m_pos = _pos;
	out->m_v = _v;
	out->m_vMax = _vMax;
	out->m_vMaxDef = _vMax;
	out->m_fMax = _fMax;
	out->m_myPack = _p;
	out->m_role = _role;
	
	
	//UE_LOG(LogTemp, Warning, TEXT("flank angle : (%f)"), out->m_flankAngle);
	return out;
}

void APredatorBoid::setFlank()
{
	if (m_role == EHuntRole::LFLANK)
	{
		m_flankAngle = m_rng.FRandRange(-120.0f, -80.0f);
		m_rng.GenerateNewSeed();
	}
	else if (m_role == EHuntRole::RFLANK)
	{
		m_flankAngle = m_rng.FRandRange(80.0f, 120.0f);
		m_rng.GenerateNewSeed();

	}
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
		onEnterRange();
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

void APredatorBoid::onEnterRange()
{
	TArray<ABoid*> prey = getPrey();
	if(prey.Num()> 0)
	{
		for(int i=0 ; i<prey.Num();++i)
		{
			if(Cast<APreyBoid>(prey[i]) == m_myPack->m_desiredPrey)
			{
				/// bite or somthing
				m_myPack->m_desiredPrey->takeDamage(0.05f*m_vMaxDef);
			}
		
		}
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
		m_vMax = FMath::Clamp(m_vMax*1.1f, 1.0f, 2.0f*m_vMaxDef);
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
	
	targetP = m_myPack->m_targetPack->m_packPos;
	/// Vector from the predator pack centre to the prey herd centre
	FVector front = m_myPack->m_pack[0]->m_pos - targetP;
	float dist = FVector::Dist(m_pos, targetP);
	bool isFar = dist - m_myPack->m_spawnRad - m_myPack->m_targetPack->m_spawnRad >=  m_collisionRad;
	
	if(isFar && m_role == EHuntRole::FRONT)
	{
		m_vMax = 0.4f*m_vMaxDef;
		FVector randRotF = FRotator(0.0f, m_rng.FRandRange(-10.0f, 10.0f), 0.0f).Vector();
		
		FVector flankPos = front * randRotF;
		m_readyToHunt = true;


		//UE_LOG(LogTemp, Warning, TEXT("DONE"));

		m_v = (m_myPack->m_targetPack->m_packPos - m_pos) * randRotF;
		m_target = wander();
	}
	else if(!isFar && !m_readyToHunt)
	{
		//UE_LOG(LogTemp, Warning, TEXT("FORMATION!"));

		performRole(targetP, front);
	}
	
	
	return seek();
}

void APredatorBoid::performRole(const FVector &_target, const FVector &_frontDir)
{
	
	FVector dir;
	FVector flankPos;
	switch (m_role)
	{
	case EHuntRole::LFLANK:
	{
		
		
		FVector randRotL = FRotator(0.0f, m_flankAngle, 0.0f).Vector();

		//UE_LOG(LogTemp, Warning, TEXT("randL : (%f)"), m_flankAngle);

		flankPos = _target + _frontDir * randRotL;
		//m_rng.GenerateNewSeed();
		dir = _frontDir.RotateAngleAxis(randRotL.Y, FVector(0.0f, 0.0f, 1.0f));
		//flankPos = _target + m_myPack->m_targetPack->m_spawnRad * dir;
		m_vMax = 1.5f*m_vMaxDef;
		
		
		break;
	}
	case EHuntRole::RFLANK:
	{
		
		
		FVector randRotR = FRotator(0.0f, m_flankAngle, 0.0f).Vector();
		//UE_LOG(LogTemp, Warning, TEXT("randR : (%f)"), r);

		flankPos = _target + _frontDir * randRotR;

		//UE_LOG(LogTemp, Warning, TEXT("getting desired prey"));
		dir = _frontDir.RotateAngleAxis(randRotR.Y, FVector(0.0f, 0.0f, 1.0f));
		//flankPos = _target + m_myPack->m_targetPack->m_spawnRad * dir;
		m_vMax = 1.5f*m_vMaxDef;
		
		break;
	}
	default:
		break;
	}

	m_v = dir - _frontDir;
	m_target = flankPos;


}



// Called every frame
void APredatorBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

