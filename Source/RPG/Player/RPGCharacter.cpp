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
#include "CharacterStat/PlayerStatComponent.h"
#include "UI/PlayerHUDWidget.h"
#include "Components/SphereComponent.h"
#include "Interaction/InteractionBase.h"
#include "UI/MenuWidget.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "Animation/AnimInstance.h"


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

	// 인풋 설정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
	if (IMCRef.Object)
	{
		DefaultMappingContext = IMCRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IAJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (IAJumpRef.Object)
	{
		JumpAction = IAJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IAJMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
	if (IAJMoveRef.Object)
	{
		MoveAction = IAJMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IAJLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
	if (IAJLookRef.Object)
	{
		LookAction = IAJLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IAJInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact'"));
	if (IAJInteractRef.Object)
	{
		InteractAction = IAJInteractRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IAMenuRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Menu.IA_Menu'"));
	if (IAMenuRef.Object)
	{
		MenuAction = IAMenuRef.Object;
	}

	// 애니메이션 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/Blueprints/Character/Player/ABP_PlayerAnim.ABP_PlayerAnim_C'"));
	if (AnimRef.Class)
	{
		AnimInstance = AnimRef.Class;
	}

	// 위젯 설정
	ConstructorHelpers::FClassFinder<UPlayerHUDWidget> HUDRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/Widget/WBP_PlayerHUD.WBP_PlayerHUD_C'"));
	if (HUDRef.Class)
	{
		HUDWidgetClass = HUDRef.Class;
	}

	ConstructorHelpers::FClassFinder<UMenuWidget> MenuRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/Widget/WBP_Menu.WBP_Menu_C'"));
	if (MenuRef.Class)
	{
		MenuWidgetClass = MenuRef.Class;
	}

	// 스탯 설정 
	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("Stat"));

	// 인터랙션 설정
	InteractionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
	InteractionRadius->SetupAttachment(GetMesh());
	InteractionRadius->SetSphereRadius(100.f);
	InteractionRadius->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	InteractionRadius->OnComponentBeginOverlap.AddDynamic(this, &ARPGCharacter::InteractionBeginOverlap);
	InteractionRadius->OnComponentEndOverlap.AddDynamic(this, &ARPGCharacter::InteractionEndOverlap);

	bMenuOpen = false;

	// 인벤토리 크기 설정
	InventorySize = 25;
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

	HUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::FromInt(ItemListInInventory.Num()));
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

		//인터랙션
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ARPGCharacter::Interaction);

		//메뉴
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ARPGCharacter::OpenMenu);
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

void ARPGCharacter::PickupItem(FItemInfo& PickupItemInfo)
{
	int32 ItemIdx = -1;
	FItemInfo FoundItem;
	bool bItemFound = false;

	for (int32 i = 0; i < ItemListInInventory.Num(); ++i)
	{
		if (PickupItemInfo.ItemClass == ItemListInInventory[i].ItemClass && PickupItemInfo.MaxStack > ItemListInInventory[i].CurrentStack)
		{
			ItemIdx = i;
			FoundItem = ItemListInInventory[i];
			bItemFound = true;
			break;
		}
	}

	// 아이템을 찾지 못한 경우 Inventory에 추가
	if (!bItemFound)
	{
		// 인벤토리에 새로운 아이템을 추가
		ItemListInInventory.Add(PickupItemInfo);
		UE_LOG(LogTemp, Warning, TEXT("New!!"));
	}
	else
	{
		// 찾은 아이템의 스택을 증가
		ItemListInInventory[ItemIdx].CurrentStack++;
		bItemFound = false;
		UE_LOG(LogTemp, Warning, TEXT("ADD!!"));
	}

	if (MenuWidget)
	{
		MenuWidget->InventoryPanel->ClearChildren();
		MenuWidget->BuildIventory();
	}
}

void ARPGCharacter::InteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		AInteractionBase* InteractableActor = Cast<AInteractionBase>(OtherActor);
		if (InteractableActor)
		{
			InteractableActors.AddUnique(InteractableActor);
			if (InteractableActors.Num() > 0)
			{
				InteractableActors[0]->Mesh->SetRenderCustomDepth(true);		
			}
		}
	}
}

void ARPGCharacter::InteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		AInteractionBase* InteractableActor = Cast<AInteractionBase>(OtherActor);
		if (InteractableActor)
		{
			InteractableActor->Mesh->SetRenderCustomDepth(false);
			InteractableActors.Remove(InteractableActor);
		}
	}
}

void ARPGCharacter::Interaction()
{
	if (InteractableActors.Num() > 0)
	{
		InteractableActors[0]->Interact(InteractableActors[0]->ItemInfo);
		
		//다시 배열체크해서 색상변경
		if (InteractableActors.Num() > 0)
		{
			InteractableActors[0]->Mesh->SetRenderCustomDepth(true);
		}
	}
}

void ARPGCharacter::OpenMenu()
{
	if (bMenuOpen == false)
	{
		if (MenuWidget == nullptr)
		{
			MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);
		}

		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(MenuWidget->TakeWidget());
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
			bMenuOpen = true;
			// 슬로우 모션 걸기
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
		}
	}
	else
	{
		MenuWidget->RemoveFromParent();
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
		bMenuOpen = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	}
}




