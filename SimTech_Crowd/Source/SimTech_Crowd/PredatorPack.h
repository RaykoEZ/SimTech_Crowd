// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PredatorBoid.h"
#include "PreyPack.h"
#include "PredatorPack.generated.h"

// Stages of the hunt
UENUM(BlueprintType)
enum class EPackStatus : uint8
{
	OBSERVING UMETA(DisplayName = "Observing"),
	HUNTING UMETA(DisplayName = "Hunting")

};



UCLASS()
class SIMTECH_CROWD_API APredatorPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APredatorPack();
	/// Leader of the pack is boid of index 0
	UPROPERTY(BlueprintReadWrite)
	TArray<APredatorBoid*> m_pack;

	/// determine where the spawn region is
	UPROPERTY(EditAnywhere)
	FVector m_spawnCentre;
	UPROPERTY(EditAnywhere)
	float m_spawnRad;
	UPROPERTY()
	float m_worldRad;
	UPROPERTY(BlueprintReadWrite)
	int m_numMember;
	UPROPERTY()
	EPackStatus m_huntStatus;
	UPROPERTY()
	APreyPack* m_targetPack;
	UPROPERTY()
	TArray<APreyBoid*> m_targetPrey;
	APreyBoid* m_desiredPrey;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void init();
	EHuntRole assignRole(const uint8 &_i);

	APreyBoid* getDesiredPrey() const;

	UPROPERTY()
	USphereComponent* m_packBound;
	UFUNCTION()
	void onBeginPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult & _sweepResult);
	UFUNCTION()
	void onEndPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor*_otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FRandomStream m_rng;
	
};
