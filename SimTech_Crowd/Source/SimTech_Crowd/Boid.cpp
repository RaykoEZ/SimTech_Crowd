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

	m_invMass = 1 / m_mass;
	m_type = EBoidType::OTHER;

	// setup root and sphere
	USphereComponent* sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	m_collision = sphereComponent;
	RootComponent = m_collision;
	sphereComponent->InitSphereRadius(200.0f);
	m_collision->bHiddenInGame = false;

	//Start delegation for sphere

	// these two are called when 1 or more actors enter/exit the sphere region
	m_collision->OnComponentBeginOverlap.AddDynamic(this, &ABoid::onBeginPresenceOverlap);
	m_collision->OnComponentEndOverlap.AddDynamic(this, &ABoid::onEndPresenceOverlap);


	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Boid Mesh"));
	//Mobility
	m_mesh->SetMobility(EComponentMobility::Movable);



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

void ABoid::updateNeighbour()
{


}

/// delegate functions
void ABoid::onBeginPresenceOverlap(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult & _sweepResult)
{
	// test
	if (_otherActor->GetActorLocation().Z < m_pos.Z || _otherComp->GetComponentLocation().Z < m_pos.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected a thing entering below me"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected a thing entering sphere"));

	}
}

void ABoid::onEndPresenceOverlap(UPrimitiveComponent * _overlappedComponent, AActor * _otherActor, UPrimitiveComponent * _otherComp, int32 _otherBodyIndex)
{

	if (_otherActor->GetActorLocation().Z < m_pos.Z || _otherComp->GetComponentLocation().Z < m_pos.Z)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected a thing exiting below me"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected a thing exiting sphere"));

	}

}


// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	update(DeltaTime);
	
}

void ABoid::printDebug()const
{
	DrawDebugLine(GetWorld(),
		m_pos,
		FVector(m_pos + 100 * m_v),
		FColor(255, 0, 0),
		false, -1, 0,
		12.333);

}

//------------------------------------------------------------------------


/// Implementations are based on this paper :
/// Steering Behaviors For Autonomous Characters
/// by Craig W.Reynolds, presented on GDC1999



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
		printDebug();
		return outV;
	}
	UE_LOG(LogTemp, Warning, TEXT("boid reached target"));
	return desiredV;
}

FVector ABoid::flee() const
{
	/// steer away from the seeking position
	FVector out = -seek();
	out.Z = 0.0f;
	return -seek();
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
	return FVector();
}

FVector ABoid::separate() const
{
	return FVector();
}

FVector ABoid::cohesion() const
{
	return FVector();
}

FVector ABoid::alignment() const
{
	return FVector();
}

FVector ABoid::avoidCollision() const
{
	return FVector();
}



