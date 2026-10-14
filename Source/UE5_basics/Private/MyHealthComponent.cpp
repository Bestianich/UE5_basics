// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5_basics/Public/MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = MaxHealth;
	
}


// Called every frame
void UMyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!AutoRecovery)
		return;
	
	timer += DeltaTime;
	if(timer > RecoveryTime)
	{
		timer = 0;
		Heal(AutoRecoveryHealAmount);
	}
}


void UMyHealthComponent::GetDamage(int amount)
{
	if (amount != 0)
	{
		const auto damage = FMath::Abs(amount);
		currentHealth = FMath::Clamp(currentHealth - amount, 0, MaxHealth);
		OnDamageTaken.Broadcast();
		timer = 0;
	}
	
	if (currentHealth <= 0)
	{
		OnHealthZero.Broadcast();
	}
}

int UMyHealthComponent::GetCurrentHealth()
{
	return currentHealth;
}

void UMyHealthComponent::Heal(int amount)
{
	const auto heal = FMath::Abs(amount);
	currentHealth = FMath::Clamp(currentHealth + amount, 0, MaxHealth);
	OnHeal.Broadcast();
}
