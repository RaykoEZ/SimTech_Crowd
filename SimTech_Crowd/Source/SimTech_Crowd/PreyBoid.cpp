// Fill out your copyright notice in the Description page of Project Settings.

#include "PreyBoid.h"


// Sets default values
APreyBoid::APreyBoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APreyBoid::BeginPlay()
{
	Super::BeginPlay();
	
}

void APreyBoid::setMesh()
{
}

void APreyBoid::update()
{
}

// Called every frame
void APreyBoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

