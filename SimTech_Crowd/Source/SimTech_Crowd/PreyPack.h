// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreyBoid.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "PreyPack.generated.h"


class APredatorPack;
// State of the prey herd
UENUM(BlueprintType)
enum class EHerdStatus : uint8
{
	NERVOUS UMETA(DisplayName = "Nervous"),
	PANIC UMETA(DisplayName = "Panic")

};


UCLASS()
class SIMTECH_CROWD_API APreyPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreyPack();

	bool m_dirty;
	/// Pack of prey boids
	/// Leader of the pack is boid of index 0
	UPROPERTY(BlueprintReadWrite)
	TArray<APreyBoid*> m_pack;
	UPROPERTY(BlueprintReadWrite)
	int m_numMember;
	/// determine where the spawn region is
	UPROPERTY(EditAnywhere)
	FVector m_spawnCentre;
	UPROPERTY(EditAnywhere)
	float m_spawnRad;
	UPROPERTY()
	float m_worldRad;
	UPROPERTY()
	FVector m_packPos;

	/// latest sighting of predator
	UPROPERTY()
	TArray<APredatorBoid*> m_threatSite;
	/// average position of the threats
	UPROPERTY()
	FVector m_threatPos;
	UPROPERTY()
	EHerdStatus m_packState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void init();

	FVector getPackPos() const;

	

	UPROPERTY()
	USphereComponent* m_packBound;
	UFUNCTION()
	void onBeginPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _fromSweep, const FHitResult & _sweepResult);
	UFUNCTION()
	void onEndPresenceOverlap(UPrimitiveComponent* _overlappedComponent, AActor*_otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// Functions to tell the herd to flee from predators in sight
	UFUNCTION()
	void updateThreat(APredatorBoid* &_predators);
	/// updates average position of the threats
	UFUNCTION()
	void updateThreatPos();

	UFUNCTION()
	void onThreatTimerUpdate();
	FTimerHandle m_threatTimer;
	
	
};
