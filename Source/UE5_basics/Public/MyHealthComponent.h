// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_BASICS_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyHealthComponent();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxHealth = 10;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FHealthDelegate OnDamageTaken;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FHealthDelegate OnHealthZero;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FHealthDelegate OnHeal;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool AutoRecovery = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int AutoRecoveryHealAmount = 10;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RecoveryTime = 2.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void GetDamage(int amount);

	UFUNCTION(BlueprintCallable)
	int GetCurrentHealth();
	
	UFUNCTION(BlueprintCallable)
	void Heal(int amount);
	
private:
	int currentHealth;
	float timer = 0;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
