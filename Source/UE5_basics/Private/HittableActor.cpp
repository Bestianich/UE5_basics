// Fill out your copyright notice in the Description page of Project Settings.


#include "HittableActor.h"

#include "MathUtil.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AHittableActor::AHittableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	
	
}

// Called when the game starts or when spawned
void AHittableActor::BeginPlay()
{
	Super::BeginPlay();
	HitCounter = 0;
	OnHit.AddDynamic(this , &AHittableActor::Hit);
}

// Called every frame
void AHittableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AHittableActor::Hit()
{
	//OnHit.Broadcast();
	HitCounter++;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Hittato!! %d" + FString::FromInt(HitCounter));
	SpawnNewBall();
}

void AHittableActor::SpawnNewBall()
{
	auto max = BallSpawnPoints.Num();
	auto index =  FMath::RandRange(0 , max);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,  "Max: " + FString::FromInt(max) + " Index: " + FString::FromInt(index));
	if (IsValid(BallSpawnPoints[index]))
		return;
	auto spawnLocation = BallSpawnPoints[index]->GetActorLocation();
	auto spawnRotation = BallSpawnPoints[index]->GetActorRotation();
	GetWorld()->SpawnActor<AActor>(BallToSpawn , spawnLocation , spawnRotation);
}


