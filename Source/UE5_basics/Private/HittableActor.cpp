// Fill out your copyright notice in the Description page of Project Settings.


#include "HittableActor.h"

#include "HittableComponent.h"
#include "MathUtil.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AHittableActor::AHittableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HittableComponent = CreateDefaultSubobject<UHittableComponent>(FName("HittableComponent"));
}

// Called when the game starts or when spawned
void AHittableActor::BeginPlay()
{
	Super::BeginPlay();
	HitCounter = 0;
	HittableComponent->OnHit.AddDynamic(this , &AHittableActor::RegisterScore);
	SpawnNewBall();
}

// Called every frame
void AHittableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AHittableActor::RegisterScore(AActor* Striker)
{
	//OnHit.Broadcast();
	
	if (!IsValid(Striker))
		return;
	auto myCharacter = Cast<AMyCharacter>(Striker);
	if (!IsValid(myCharacter) || !myCharacter->GetHasBall())
		return;
	HitCounter++;
	myCharacter->LoseBall(nullptr);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Hittato!! %d" + FString::FromInt(HitCounter));
	SpawnNewBall();
}

 
void AHittableActor::SpawnNewBall()
{
	auto max = BallSpawnPoints.Num();
	auto index =  FMath::RandRange(0 , max-1);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,  "Max: " + FString::FromInt(max) + " Index: " + FString::FromInt(index));
	// if (IsValid(BallSpawnPoints[index]))
	// 	return;
	auto spawnLocation = BallSpawnPoints[index]->GetActorLocation();
	auto spawnRotation = BallSpawnPoints[index]->GetActorRotation();
	if (!IsValid(CurrentBall))
		CurrentBall = GetWorld()->SpawnActor<AABall>(BallToSpawn , spawnLocation , spawnRotation);
	CurrentBall->SetActorLocation(spawnLocation);
	CurrentBall->ToggleCollision(true);
	
}


