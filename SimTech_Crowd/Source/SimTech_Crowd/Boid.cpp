// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"


// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoid::setMesh()
{
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

