// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/PlayerHUDWidget.h"


//////////////////////////////////////////////////////////////////////////
// ARPGCharacter

ARPGCharacter::ARPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(26.f, 96.0f);
		
	// 캐릭터 메시 설정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.000856f, -96.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/ Script / Engine.SkeletalMesh'/Game/ThirdPerson/Blueprints/Character/Player/Components/akai_e_espiritu_2_.akai_e_espiritu_2_'"));
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

	// 무브먼트 설정
	GetCharacterMovement()->MaxAcceleration = 768.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.75f;
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	ConstructorHelpers::FClassFinder<UPlayerHUDWidget> HUDRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/HUD/WBP_PlayerHUD.WBP_PlayerHUD_C'"));
	if (HUDRef.Class)
	{
		CSHUDWidgetClass = HUDRef.Class;
	}

	// 스탯 초기화
	Level = 1;
	CurrentHealth = 100.f;
	MaxHealth = 100.f;
	CurrentMana = 100.f;
	MaxMana = 100.f;
	CurrentXP = 0.f;
	NextLevelXP = 50.f;
	Strength = 10;
	Defense = 10;
	SwordModifier = 0;
	ShieldModifier = 0;
}

void ARPGCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	CSHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), CSHUDWidgetClass);
	CSHUDWidget->AddToViewport();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Look);

	}

}

void ARPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




