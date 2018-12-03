// Fill out your copyright notice in the Description page of Project Settings.

#include "SimWorldPawn.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

// Sets default values
ASimWorldPawn::ASimWorldPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->bHiddenInGame = true;
	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;


}

// Called when the game starts or when spawned
void ASimWorldPawn::BeginPlay()
{
	Super::BeginPlay();
	
	initSim();

	
}

// Called every frame
/// Remember to use timer for simulation-----------------------------------------------
void ASimWorldPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASimWorldPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASimWorldPawn::initSim()
{
	if (GetWorld())
	{
		m_test = GetWorld()->SpawnActor<ABoid>(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f));
		m_test->m_target = FVector(100.0f, -100.0f, 0.0f);
		//m_test->SetActorLocation(FVector(0.0f, 0.0f, 10.0f));
		m_test->m_pos = m_test->GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("m_pos : (%f , %f, %f)"), m_test->m_pos.X, m_test->m_pos.Y, m_test->m_pos.Z);

		m_test->m_v = FVector(0.0f, 1.0f, 0.0f);
		m_test->m_vMax = 1.0f;
		m_test->m_fMax = 1.0f;

	}
}





