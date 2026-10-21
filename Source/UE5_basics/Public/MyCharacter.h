// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "UE5_basics/Public/MyHealthComponent.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class UEnhancedInputComponent;

UCLASS()
class UE5_BASICS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveVector* OffsetCurve;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	UMyHealthComponent* HealthComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* StartFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Ball;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float StealRange = 1000;
	
	UFUNCTION(BlueprintCallable)
	void TryStealBall();
	
	UFUNCTION(BlueprintCallable)
	void StealBall(AMyCharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	void LoseBall();
	
	
	UFUNCTION(BlueprintCallable)
	void FireFromWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FTimeline AimTimeline;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	virtual void NotifyControllerChanged() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBall = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AimIn();
	void AimOut();
	
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

private:
	UFUNCTION()
	void HandleProgressArmLength(float Length);
	
	UFUNCTION()
	void HandleProgressCameraOffset(FVector Offset);
};
