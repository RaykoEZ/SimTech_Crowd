// Fill out your copyright notice in the Description page of Project Settings.

#include "SimWorldPawn.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
ASimWorldPawn::ASimWorldPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimWorldPawn::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()) 
	{
		m_test = GetWorld()->SpawnActor<ABoid>(FVector(10.0f, 10.0f, 0.1f), FRotator(0.0f));
		m_test->m_target = FVector(20.0f, 10.0f, 0.1f);
		m_test->m_pos = m_test->GetActorLocation();
		m_test->m_vMax = 20.0f;
		m_test->m_fMax = 20.0f;		

	}


	
}

// Called every frame
void ASimWorldPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASimWorldPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

