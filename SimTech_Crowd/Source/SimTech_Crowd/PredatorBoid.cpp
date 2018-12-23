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
	FVector desiredV = m_target - m_pos;
	FVector outV = desiredV.GetSafeNormal();

	FVector accel = f * m_invMass;
	FVector oldV = m_v + accel;
	m_v = ClampVector(oldV, FVector(-m_vMax * 0.5f), FVector(m_vMax));
	m_pos += m_v;
	m_mesh->SetWorldLocation(m_pos);
	RootComponent->SetWorldLocation(m_pos);
	printDebug(FColor(255.0f, 0.0, 0.0f));
}

void APredatorBoid::handleStatus()
{
}

FVector APredatorBoid::followLead() const
{
	return FVector();
}

// Called every frame
void APredatorBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

