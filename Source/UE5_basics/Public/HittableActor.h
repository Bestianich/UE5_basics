// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HittableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitDelegate);

UCLASS()
class UE5_BASICS_API AHittableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHittableActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	int HitCounter = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> BallSpawnPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BallToSpawn;
	
	UPROPERTY(BlueprintAssignable)
	FOnHitDelegate OnHit;
	
	UFUNCTION(BlueprintCallable)
	void Hit();
	UFUNCTION(BlueprintCallable)
	void SpawnNewBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
