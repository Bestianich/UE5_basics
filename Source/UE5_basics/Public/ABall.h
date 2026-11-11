// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ABall.generated.h"

UCLASS()
class UE5_BASICS_API AABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABall();
	
	
	
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	USphereComponent* SphereComponent;
	
	
	
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	
	void ToggleCollision(bool enable) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
