// Fill out your copyright notice in the Description page of Project Settings.


#include "ABall.h"

#include "MyCharacter.h"

// Sets default values
AABall::AABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
}

// Called when the game starts or when spawned
void AABall::BeginPlay()
{
	Super::BeginPlay();
	ToggleCollision(true);
}

// Called every frame
void AABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AABall::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	
	if (!IsValid(OtherActor))
		return;
	
	AMyCharacter* character = dynamic_cast<AMyCharacter*>(OtherActor);
	
	if (!IsValid(character))
		return;
	
	ToggleCollision(false);
	character->GetBall(this);
}



void AABall::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	
	
}


void AABall::ToggleCollision(bool enable) const
{
	if (enable)
		this->SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		this->SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
