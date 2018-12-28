// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimalTrack.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


// Sets default values
AAnimalTrack::AAnimalTrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAnimalTrack::BeginPlay()
{
	Super::BeginPlay();
	printDebug(FColor(0.0f));
}

// Called every frame
void AAnimalTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAnimalTrack::printDebug(const FColor & _c) const
{
	//UE_LOG(LogTemp, Warning, TEXT("Draw"));
	DrawDebugLine(GetWorld(),
		m_pos,
		FVector(m_pos + 50 * m_pointTo),
		_c,
		true, -1.0f, 0,
		30.0f);
}

