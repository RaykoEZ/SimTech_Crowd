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
	//ABoid::update(_dt);
	/// force we are using in different cases
	FVector f;
	/// do things according to your thing
	
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
			float dist = FVector::Dist(m_pos, m_myPack->m_pack[0]->m_pos);
			if (dist > m_myPack->m_packRad)
			{
				m_status = EBoidStatus::REGROUP;
				//UE_LOG(LogTemp, Warning, TEXT("Wandering!"));
			}
			else
			{
				m_target = wander();
				f = seek();
				//UE_LOG(LogTemp, Warning, TEXT("Wandering!"));
			}
			
			break;
		}
		case EBoidStatus::PURSUING:
		{
			m_vMax = FMath::Clamp(m_vMax*1.3f, 1.0f, 4.0f);
			m_target = cohesion(EBoidType::PREY);
			f = seek();
			//UE_LOG(LogTemp, Warning, TEXT("Pursue!"));

			break;
		}
		case EBoidStatus::REGROUP:
		{
			float dist = FVector::Dist(m_pos, m_myPack->m_pack[0]->m_pos);
			float distO = FVector::Dist(m_pos, FVector(0.0f));

			//UE_LOG(LogTemp, Warning, TEXT("Dist %f"), dist);
			//UE_LOG(LogTemp, Warning, TEXT("Dist %f"), distO);
			//UE_LOG(LogTemp, Warning, TEXT("PackRad %f"), m_myPack->m_packRad);
			//UE_LOG(LogTemp, Warning, TEXT("WRad %f"), m_myPack->m_worldRad);


			if(dist > m_myPack->m_packRad || distO > m_myPack->m_worldRad)
			{
				m_target = regroup();
				f = seek();
				//UE_LOG(LogTemp, Warning, TEXT("REGROUP!"));
			}
			else
			{
				m_isOutOfBound = false;
				m_status = EBoidStatus::WANDERING;
				//UE_LOG(LogTemp, Warning, TEXT("REGROUPed, now wandering!"));
			}
			

			break;
		}
		case EBoidStatus::DEAD:
		{
			f = FVector(0.0f);
			m_v = FVector(0.0f);
			m_target = m_pos;
			break;
		}
		default:

			break;
	}

	FVector desiredV = m_target - m_pos;
	FVector outV = desiredV.GetSafeNormal();

	FVector accel = f * m_invMass;
	FVector oldV = m_v + accel;
	m_v = ClampVector(oldV, FVector(-m_vMax,-m_vMax,0.0f), FVector(m_vMax, m_vMax,0.0f));
	m_pos += m_v;
	/// Update visuals
	m_mesh->SetWorldLocation(m_pos);
	RootComponent->SetWorldLocation(m_pos);
	printDebug(FColor(255.0f, 0.0, 0.0f));
}




void APredatorBoid::handleStatus()
{
	/// Predators lost preys
	if (m_isOutOfBound)
	{
		m_status = EBoidStatus::REGROUP;	
		//m_isOutOfBound = false;
		//UE_LOG(LogTemp, Warning, TEXT("Out of hunting ground!"));

		return;
	}
	if (searchPrey().Num() > 0)
	{
		
		m_status = EBoidStatus::PURSUING;
		//UE_LOG(LogTemp, Warning, TEXT("Found prey!"));
		//m_vMax *= 1.15f;
		
	}
	else 
	{
		m_status = EBoidStatus::WANDERING;
		//UE_LOG(LogTemp, Warning, TEXT("Wandering!"));

	}

}

FVector APredatorBoid::followLead() const
{
	return FVector();
}

FVector APredatorBoid::regroup()
{
	// get a random position in leader's direction


	if (m_myPack->m_pack[0] != this)
	{
		FVector bossTarget = m_myPack->m_pack[0]->wander();
		float r = m_myPack->m_packRad;
		m_vMax = FMath::Clamp(m_vMax*1.5f, 1.0f, 5.0f);

		FVector aprxTarget = bossTarget +
			FVector(FMath::RandRange(-r, r),
				FMath::RandRange(-r, r), 0.0f);
		return aprxTarget;
	}
	m_vMax = m_vMaxDef;
	m_v =  - m_pos.GetSafeNormal();
	return wander();
}

// Called every frame
void APredatorBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

