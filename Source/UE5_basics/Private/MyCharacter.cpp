// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5_basics/Public/MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HittableActor.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	HealthComp = CreateDefaultSubobject<UMyHealthComponent>(FName("Health Component"));
	StartFire = CreateDefaultSubobject<USceneComponent>(FName("StartFire"));
	
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ball"));
	Ball->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Ball->SetVisibility(HasBall);
	
	if(MovementCurve)
	{
		FOnTimelineFloat ProgressFunctionLength;
		ProgressFunctionLength.BindUFunction(this, "HandleProgressArmLength");
		AimTimeline.AddInterpFloat(MovementCurve, ProgressFunctionLength);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Silver, "Bind the timeline!");
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTimeline.TickTimeline(DeltaTime);
	

}


void AMyCharacter::TryStealBall()
{
	FCollisionQueryParams Params;
	const auto MyWorld = GetWorld();
	if (!IsValid(MyWorld))
		return;
	
	Params.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(MyWorld, 0));
	FHitResult Hit;
	
	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + (FollowCamera->GetForwardVector() * StealRange);
	
	bool bHit = MyWorld->LineTraceSingleByChannel(Hit, StartLocation, EndLocation,ECC_Camera , Params);
	DrawDebugLine(MyWorld, StartLocation, EndLocation, FColor::Red, false, 0.0f, 0, 1.0f);

	if (!bHit)
		return;
	
	auto hittedActor = Cast<AMyCharacter>(Hit.GetActor());
	
	if (!IsValid(hittedActor))
		return;
	
	if (!hittedActor->HasBall)
	{	
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Niente da rubare!!");
		return;
	}
	
	StealBall(hittedActor);
}

void AMyCharacter::StealBall(AMyCharacter* Character)
{
	if (IsValid(Character))
		Character->LoseBall();
	HasBall = true;
	Ball->SetVisibility(true);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Rubato!!");
}

void AMyCharacter::LoseBall()
{
	HasBall = false;
	Ball->SetVisibility(false);
}

void AMyCharacter::FireFromWeapon()
{
	FCollisionQueryParams Params;
	const auto MyWorld = GetWorld();
	if (!IsValid(MyWorld))
		return;
	
	Params.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(MyWorld, 0));
	FHitResult Hit;
	
	float WeaponRange = 2000;
	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + (FollowCamera->GetForwardVector() *WeaponRange);
	
	bool bHit = MyWorld->LineTraceSingleByChannel(Hit, StartLocation, EndLocation,ECC_Camera , Params);
	DrawDebugLine(MyWorld, StartLocation, EndLocation, FColor::Green, false, 0.0f, 0, 1.0f);

	if (!bHit)
		return;
	
	auto hittedActor = Cast<AHittableActor>(Hit.GetActor());
	
	if (!IsValid(hittedActor))
		return;
	
	if (!HasBall)
		return;
	
	hittedActor->Hit();
	LoseBall();
}
#pragma region Movement

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);

		// Aim
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMyCharacter::AimIn);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMyCharacter::AimOut);
		
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMyCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMyCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMyCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AMyCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AMyCharacter::AimIn()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	AimTimeline.Play();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, "Aim IN");
}

void AMyCharacter::AimOut()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	AimTimeline.Reverse();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, "Aim OUT");
}

void AMyCharacter::HandleProgressArmLength(float Length)
{
	CameraBoom->TargetArmLength = Length;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("New camer boom val -> %f"), Length));
}

void AMyCharacter::HandleProgressCameraOffset(FVector Offset) {
	CameraBoom->SocketOffset = Offset;
}


void AMyCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (!DefaultMappingContext)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "DefaultMappingContext e' NULL!");
		return;
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "IMC aggiunto");
		}
	}
}

#pragma endregion Movement


