// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"



// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//m_invMass = 1 / m_mass;
	m_type = EBoidType::OTHER;
	m_status = EBoidStatus::WANDERING;
	// setup root and sphere
	USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootScene"));
	m_collision = sphereComponent;
	RootComponent = m_collision;
	//m_collisionRad = 1000.0f;
	sphereComponent->InitSphereRadius(m_collisionRad);
	m_collision->bHiddenInGame = false;

	//Start delegation for sphere

	// these two are called when 1 or more actors enter/exit the sphere region
	m_collision->OnComponentBeginOverlap.AddDynamic(this, &ABoid::onBeginPresenceOverlap);
	m_collision->OnComponentEndOverlap.AddDynamic(this, &ABoid::onEndPresenceOverlap);
	m_collision->ShapeColor = FColor(255.0f, 255.0f, 255.0f);

	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Boid Mesh"));
	//Mobility
	m_mesh->SetMobility(EComponentMobility::Movable);
	
	m_isOutOfBound = false;

	int32 seed = FMath::Rand();
	m_rng.Initialize(seed);

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	m_invMass = 1.0f / m_mass;
	//UE_LOG(LogTemp, Warning, TEXT("m_pos : (%f , %f, %f)"), m_pos.X, m_pos.Y, m_pos.Z);
	//UE_LOG(LogTemp, Warning, TEXT("RootLoc : (%f , %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
}


/// movement of boid every frame
void ABoid::update(const float &_dt)
{
	// test for seek and flee

	FVector desiredV = m_target - m_pos;
	FVector outV = desiredV.GetSafeNormal();
	//FVector f = flee();

	FVector f = seek();

	FVector accel = f * m_invMass;
	FVector oldV = m_v + accel;
	m_v = ClampVector(oldV, FVector(-m_vMax*0.5f), FVector(m_vMax));
	m_pos += m_v;
	m_mesh->SetWorldLocation(m_pos);
	RootComponent->SetWorldLocation(m_pos);
	//SetActorLocation(m_pos);
	//UE_LOG(LogTemp, Warning, TEXT("m_pos : (%f , %f, %f)"), m_pos.X, m_pos.Y, m_pos.Z);
	//UE_LOG(LogTemp, Warning, TEXT("dir : (%f , %f, %f)"), m_v.X, m_v.Y, m_v.Z);
	

}

void ABoid::handleStatus()
{
}

void ABoid::onEnterRange()
{
}




/// delegate functions
void ABoid::onBeginPresenceOverlap(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult & _sweepResult)
{
	ABoid* intruder = Cast<ABoid>(_otherActor);
	if(intruder!=nullptr)
	{
		m_neighbours.Add(intruder);

		//UE_LOG(LogTemp, Warning, TEXT("+1 Intruder Boid: %d boids in"),m_neighbours.Num());
	}
}

void ABoid::onEndPresenceOverlap(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComp, int32 _otherBodyIndex)
{
	for(int i=0 ; i<m_neighbours.Num(); ++i)
	{	
		if(m_neighbours[i]==_otherActor)
		{
			m_neighbours.RemoveAt(i);
			//UE_LOG(LogTemp, Warning, TEXT("-1 Intruder Boid: %d boids in"), m_neighbours.Num());
			return;
		}
	
	}
	
}


// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	update(DeltaTime);
	
}


TArray<int> ABoid::searchPrey() const
{
	TArray<int> out;
	for(int i = 0; i < m_neighbours.Num(); ++i)
	{
		if (m_neighbours[i]->m_type == EBoidType::PREY) 
		{
			out.Add(i);
		}
	}

	return out;
}

TArray<int> ABoid::searchPredator() const
{
	TArray<int> out;
	for (int i = 0; i < m_neighbours.Num(); ++i)
	{
		if (m_neighbours[i]->m_type == EBoidType::PREDATOR)
		{
			out.Add(i);
		}
	}

	return out;
}

TArray<ABoid*> ABoid::getPredator() const
{
	TArray<ABoid*> out;
	for (int i = 0; i < m_neighbours.Num(); ++i)
	{
		if (m_neighbours[i]->m_type == EBoidType::PREDATOR)
		{
			out.Add(m_neighbours[i]);
		}
	}

	return out;
}
TArray<ABoid*> ABoid::getPrey() const
{
	TArray<ABoid*> out;
	for (int i = 0; i < m_neighbours.Num(); ++i)
	{
		if (m_neighbours[i]->m_type == EBoidType::PREY)
		{
			out.Add(m_neighbours[i]);
		}
	}

	return out;
}

void ABoid::printDebug(const FColor &_c)const
{
	//UE_LOG(LogTemp, Warning, TEXT("Draw"));
	DrawDebugLine(GetWorld(),
		m_pos,
		FVector(m_pos + 120 * m_v),
		_c,
		false, 0.1f, 0,
		120.0f);

}

//------------------------------------------------------------------------


/// Implementations from ideas based on this paper :
/// Steering Behaviors For Autonomous Characters
/// by Craig W.Reynolds, presented on GDC1999

void ABoid::resolve(const FVector &_f)
{
	
	FVector desiredV = m_target - m_pos;
	FVector outV = desiredV.GetSafeNormal();
	FVector accel = _f * m_invMass;
	FVector oldV = m_v + accel;
	
	m_v = ClampVector(oldV, FVector(-m_vMax, -m_vMax, 0.0f), FVector(m_vMax, m_vMax, 0.0f));
	
	m_pos += m_v;
	/// Update visuals
	m_mesh->SetWorldLocation(m_pos);
	RootComponent->SetWorldLocation(m_pos);
	//
}


/// Seek a position to steer towards
FVector ABoid::seek() const
{
	FVector desiredV = m_target - m_pos;
	FVector outV = desiredV.GetSafeNormal();
	if (!FMath::IsNearlyEqual(outV.Size(),100.0f))
	{

		outV *= m_vMax;
		outV -= m_v;
	
		outV.Z = 0.0f;
		// Draw direction line for debug
		
		return outV;
	}
	//UE_LOG(LogTemp, Warning, TEXT("boid reached target"));
	return desiredV;
}

FVector ABoid::flee()
{
	/// steer away from the seeking position
	
	FVector desiredV =  m_pos - m_target;
	FVector outV = desiredV.GetSafeNormal();
	if (!FMath::IsNearlyEqual(outV.Size(), 100.0f))
	{

		outV *= m_vMax;
		outV -= m_v;

		outV.Z = 0.0f;
		// Draw direction line for debug

		return outV;
	}
	//UE_LOG(LogTemp, Warning, TEXT("boid reached target"));
	return -m_v;
}



FVector ABoid::pursue(const FVector &_futureP) const
{
	FVector desiredV = m_pos - _futureP;

	FVector outV = desiredV.GetSafeNormal();

	outV *= m_vMax;
	outV -= m_v;
	return outV;
}

FVector ABoid::wander() const
{
	
	FVector future = m_pos + 10.0f * m_v;
	FVector randRot = FRotator(0.0f, FMath::RandRange(-180.0f, 180.0f), 0.0f).Vector();
	FVector randPos = future + 5.0f * randRot;
	
	return randPos;
}
/// same with wander but gets reverse positions
FVector ABoid::turnBack() const
{
	FVector future = m_pos - 10.0f * m_v;
	FVector randRot = FRotator(0.0f, FMath::RandRange(-180.0f, 180.0f), 0.0f).Vector();
	FVector randPos = future + 5.0f * randRot;

	return randPos;
}

FVector ABoid::separate()
{
	float comfortDist = 10.0f*m_collisionRad;
	FVector newV;
	if (m_neighbours.Num() > 0) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("separate"));

		for (int i = 0; i < m_neighbours.Num(); ++i)
		{
			float dist = FVector::Dist(m_pos, m_neighbours[i]->m_pos);
			if (dist < comfortDist)
			{
				//UE_LOG(LogTemp, Warning, TEXT("too close"));
				FVector diffV = (m_pos - m_neighbours[i]->m_pos );
				diffV /= dist;
				newV += diffV;
			}
		}
		newV = (newV / m_neighbours.Num());
		m_vMax = 1.5f*m_vMaxDef;
		return FVector(newV.X, newV.Y, 0.0f);
	
	}
	m_vMax = 1.0f*m_vMaxDef;
	return FVector(0.0f);
}

FVector ABoid::getAverageNeighbourPos(const EBoidType &_t)
{
	TArray<int> idx;
	FVector newP = FVector(0.0f);

	if (_t == EBoidType::PREDATOR)
	{
		idx = searchPredator();

	}
	else
	{
		idx = searchPrey();

	}

	if (idx.Num() > 0)
	{
		
		for (int i = 0; i < idx.Num(); ++i)
		{
			newP += m_neighbours[idx[i]]->m_pos;
		}
		newP /= idx.Num();
		//UE_LOG(LogTemp, Warning, TEXT("newP : (%f , %f, %f)"), newP.X, newP.Y, newP.Z);

		//m_target = newP;
		return newP;
	}
	return m_target;
}

FVector ABoid::alignment()
{
	TArray<int> idx;
	FVector newV = FVector(0.0f);
	if (idx.Num() < 0)
	{
		switch (m_type)
		{
		case EBoidType::PREDATOR:
		{
			idx = searchPredator();
			break;
		}
		case EBoidType::PREY:
		{
			idx = searchPrey();
			break;
		}
		default:
			break;
		}

		for (int i = 0; i < idx.Num(); ++i)
		{
			newV += m_neighbours[idx[i]]->m_v;
		}
		newV /= idx.Num();
		return newV;
	}
	return m_v;
}




