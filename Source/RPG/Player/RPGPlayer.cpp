// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/RPGPlayer.h"
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
#include "Misc/OutputDeviceNull.h"
#include "Components/SceneComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ARPGPlayer

ARPGPlayer::ARPGPlayer()
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
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 0.f;

	// 무브먼트 설정
	GetCharacterMovement()->MaxAcceleration = 768.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.75f;
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 공격 컴포넌트
	AttackFrom = CreateDefaultSubobject<USceneComponent>(TEXT("AttackFrom"));
	AttackFrom->SetupAttachment(RootComponent);

	FireFrom = CreateDefaultSubobject<USceneComponent>(TEXT("FireFrom"));
	FireFrom->SetupAttachment(FollowCamera);
	FireFrom->SetRelativeLocation(FVector(110.0f, -52.0f, 4.0f));

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


	static ConstructorHelpers::FObjectFinder<UInputAction> IADrawSwordRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_DrawSword.IA_DrawSword'"));
	if (IADrawSwordRef.Object)
	{
		DrawSwordAction = IADrawSwordRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IADrawBowRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_DrawBow.IA_DrawBow'"));
	if (IADrawBowRef.Object)
	{
		DrawBowAction = IADrawBowRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackdRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Attack.IA_Attack'"));
	if (AttackdRef.Object)
	{
		AttackAction = AttackdRef.Object;
	}

	// 애니메이션 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ThirdPerson/Blueprints/Character/Player/ABP_PlayerAnim.ABP_PlayerAnim_C'"));
	if (AnimRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimRef.Class);
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

	// 화살 설정
	ConstructorHelpers::FClassFinder<AInteractionBase> BPArrowRef(TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/Interaction/Weapons/BP_WoodArrow.BP_WoodArrow_C'"));
	if (BPArrowRef.Succeeded())
	{
		ArrowClass = BPArrowRef.Class;
	}

	// 스탯 설정 
	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("Stat"));

	// 인터랙션 설정
	InteractionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRadius"));
	InteractionRadius->SetupAttachment(GetMesh());
	InteractionRadius->SetSphereRadius(150.f);
	InteractionRadius->SetCollisionProfileName(TEXT("PlayerInteraction"));
	InteractionRadius->OnComponentBeginOverlap.AddDynamic(this, &ARPGPlayer::InteractionBeginOverlap);
	InteractionRadius->OnComponentEndOverlap.AddDynamic(this, &ARPGPlayer::InteractionEndOverlap);

	bCanAttack = true;

	// 인벤토리 크기 설정
	InventorySize = 25;
}

void ARPGPlayer::BeginPlay()
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
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARPGPlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayer::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGPlayer::Look);

		//인터랙션
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ARPGPlayer::Interaction);

		//메뉴
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ARPGPlayer::OpenMenu);

		//칼 장착
		EnhancedInputComponent->BindAction(DrawSwordAction, ETriggerEvent::Completed, this, &ARPGPlayer::DrawSword);

		//활 장착
		EnhancedInputComponent->BindAction(DrawBowAction, ETriggerEvent::Completed, this, &ARPGPlayer::DrawBow);

		//공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ARPGPlayer::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ARPGPlayer::CancelAttack);
	}

}

void ARPGPlayer::Move(const FInputActionValue& Value)
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

void ARPGPlayer::Look(const FInputActionValue& Value)
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

void ARPGPlayer::Jump()
{
	if (!bIsChangingEquipment && !bAttackSaved && !bFireArrow)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void ARPGPlayer::DrawSword()
{
	if (SwordInfo.ItemClass && !bBowDrawn)
	{
		if (!bIsChangingEquipment)
		{
			if (bSwordDrawn)
			{
				bSwordDrawn = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
				GetCharacterMovement()->bUseControllerDesiredRotation = false;
				MeleeCamReShift();
				AttackType = EAttackType::None;
			}
			else
			{
				bSwordDrawn = true;
				GetCharacterMovement()->bOrientRotationToMovement = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
				MeleeCamShift();
				AttackType = EAttackType::Melee;
			}
		}

		bIsChangingEquipment = true;
	}
}

void ARPGPlayer::DrawBow()
{
	if (bHeld)
	{
		bCancel = true;
		bCanAttack = true;
		bFireArrow = false;

		FTimerHandle DelayHandle;
		float DelayTime = 0.1; //시간을 설정하고
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
			{
				bCancel = false;

			}), DelayTime, false);
	}
	else
	{
		if (BowInfo.ItemClass)
		{
			if (!bIsChangingEquipment && !GetCharacterMovement()->IsFalling() && !bSwordDrawn)
			{
				if (bBowDrawn)
				{
					bBowDrawn = false;
					GetCharacterMovement()->bOrientRotationToMovement = true;
					GetCharacterMovement()->bUseControllerDesiredRotation = false;
					BowCamReShift();
					AttackType = EAttackType::None;
				}
				else
				{
					bBowDrawn = true;
					GetCharacterMovement()->bOrientRotationToMovement = false;
					GetCharacterMovement()->bUseControllerDesiredRotation = true;
					BowCamShift();
					AttackType = EAttackType::Ranged;
				}
				bIsChangingEquipment = true;
			}
		}
	}


}

void ARPGPlayer::PickupItem(const FItemInfo& PickupItemInfo)
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

void ARPGPlayer::InteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetOwner()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		AInteractionBase* InteractableActor = Cast<AInteractionBase>(OtherComp->GetOwner());
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

void ARPGPlayer::InteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetOwner()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		AInteractionBase* InteractableActor = Cast<AInteractionBase>(OtherComp->GetOwner());
		if (InteractableActor)
		{
			InteractableActor->Mesh->SetRenderCustomDepth(false);
			InteractableActors.Remove(InteractableActor);
		}
	}
}

void ARPGPlayer::Interaction()
{
	if (InteractableActors.Num() > 0)
	{	
		InteractableActors[0]->Interact();

		//다시 배열체크해서 색상변경
		if (InteractableActors.Num() > 0)
		{
			InteractableActors[0]->Mesh->SetRenderCustomDepth(true);
		}
	}
}

void ARPGPlayer::OpenMenu()
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

void ARPGPlayer::UseItem(const FItemInfo& ItemInfo)
{
	switch (ItemInfo.ItemType)
	{
	case EItemType::None:

		break;

	case EItemType::HPItem:

		RestoreHP(ItemInfo.Potency);
		RemoveItem(ItemInfo);
		break;

	case EItemType::MPItem:

		RestoreMP(ItemInfo.Potency);
		RemoveItem(ItemInfo);
		break; 

	case EItemType::Sword:

		ChangeSword(ItemInfo);
		if (SwordRef)
		{
			DestroyGear();
			SpawnGear();
		}

		RemoveItem(ItemInfo);

		break; 

	case EItemType::Shield:

		ChangeShield(ItemInfo);
		if (ShieldRef)
		{
			DestroyGear();
			SpawnGear();
		}

		RemoveItem(ItemInfo);

		break; 

	case EItemType::Accessory:

		break;

	case EItemType::Bow:

		EquipBow(ItemInfo);
		RemoveItem(ItemInfo);
		break;

	case EItemType::Arrows:

		break;
	case EItemType::KeyItem:

		break;

	case EItemType::Resources:

		break;
	}
}

void ARPGPlayer::RemoveItem(const FItemInfo& ItemInfo)
{
	int32 FindItemIdx = ItemListInInventory.Find(ItemInfo);

	if (ItemListInInventory[FindItemIdx].CurrentStack <= 1)
	{
		ItemListInInventory.RemoveAt(FindItemIdx);
		// ItemSelected 초기화 하기
		ItemSelected = FItemInfo();
	}
	else
	{
		ItemListInInventory[FindItemIdx].CurrentStack -= 1;
	}

	if (MenuWidget)
	{
		MenuWidget->InventoryPanel->ClearChildren();
		MenuWidget->BuildIventory();
	}
}

void ARPGPlayer::RestoreHP(float RestoreAmount)
{
	Stat->CurrentHp += RestoreAmount;
	if (Stat->CurrentHp >= Stat->MaxHp)
	{
		Stat->CurrentHp = Stat->MaxHp;
	}
}

void ARPGPlayer::RestoreMP(float RestoreAmount)
{
	Stat->CurrentMp += RestoreAmount;
	if (Stat->CurrentMp >= Stat->MaxMp)
	{
		Stat->CurrentMp = Stat->MaxMp;
	}
}

void ARPGPlayer::ChangeSword(const FItemInfo& ItemInfo)
{
	if (SwordInfo.ItemClass)
	{
		PickupItem(SwordInfo);
	}
	
	SwordInfo = ItemInfo;
	SwordModifier = SwordInfo.Potency;

	if (MenuWidget)
	{
		MenuWidget->CheckBtnStyle();
	}
}

void ARPGPlayer::ChangeShield(const FItemInfo& ItemInfo)
{
	if (ShieldInfo.ItemClass)
	{
		PickupItem(ShieldInfo);
	}

	ShieldInfo = ItemInfo;
	ShieldModifier = ShieldInfo.Potency;

	if (MenuWidget)
	{
		MenuWidget->CheckBtnStyle();
	}
}

void ARPGPlayer::ChangeAcc(const FItemInfo& ItemInfo)
{
	AccInfo = ItemInfo;
}

void ARPGPlayer::EquipBow(const FItemInfo& ItemInfo)
{
	if (BowInfo.ItemClass)
	{
		PickupItem(BowInfo);
	}

	BowInfo = ItemInfo;

	if (MenuWidget)
	{
		MenuWidget->CheckBtnStyle();
	}
}

void ARPGPlayer::SpawnGear()
{
	if (bBowDrawn)
	{
		if (BowInfo.ItemClass)
		{
			BowRef = GetWorld()->SpawnActor<AInteractionBase>(BowInfo.ItemClass, GetMesh()->GetSocketTransform(TEXT("BowSlot"), ERelativeTransformSpace::RTS_Actor));

			BowRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BowSlot"));

			BowRef->Sphere->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}
	else
	{
		if (SwordInfo.ItemClass)
		{
			SwordRef = GetWorld()->SpawnActor<AInteractionBase>(SwordInfo.ItemClass, GetMesh()->GetSocketTransform(TEXT("WeaponSlot"), ERelativeTransformSpace::RTS_Actor));

			SwordRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSlot"));

			SwordRef->Sphere->SetCollisionProfileName(TEXT("NoCollision"));
		}

		if (ShieldInfo.ItemClass)
		{
			ShieldRef = GetWorld()->SpawnActor<AInteractionBase>(ShieldInfo.ItemClass, GetMesh()->GetSocketTransform(TEXT("ShieldSlot"), ERelativeTransformSpace::RTS_Actor));

			ShieldRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("ShieldSlot"));

			ShieldRef->Sphere->SetCollisionProfileName(TEXT("NoCollision"));
		}
	}

	bItemEquip = true;
}

void ARPGPlayer::DestroyGear()
{
	if (SwordRef)
	{
		SwordRef->Destroy();
	}

	if (ShieldRef)
	{
		ShieldRef->Destroy();
	}

	if (BowRef)
	{
		BowRef->Destroy();
	}
	
	bItemEquip = false;
}

//블루프린트 함수 호출하기
void ARPGPlayer::MeleeCamShift()
{
	FString FuncName = TEXT("MeleeCamShift");
	FOutputDeviceNull Ar;
	this->CallFunctionByNameWithArguments(*FuncName, Ar, NULL, true);
}

//블루프린트 함수 호출하기
void ARPGPlayer::MeleeCamReShift()
{
	FString FuncName = TEXT("MeleeCamReShift");
	FOutputDeviceNull Ar;
	this->CallFunctionByNameWithArguments(*FuncName, Ar, NULL, true);
}

void ARPGPlayer::BowCamShift()
{
	FString FuncName = TEXT("BowCamShift");
	FOutputDeviceNull Ar;
	this->CallFunctionByNameWithArguments(*FuncName, Ar, NULL, true);
}

void ARPGPlayer::BowCamReShift()
{
	FString FuncName = TEXT("BowCamReShift");
	FOutputDeviceNull Ar;
	this->CallFunctionByNameWithArguments(*FuncName, Ar, NULL, true);
}

void ARPGPlayer::Attack()
{
	if (!bIsChangingEquipment && !GetCharacterMovement()->IsFalling())
	{
		switch (AttackType)
		{
		case EAttackType::None:
			break;
		case EAttackType::Melee:
			if (bCanAttack)
			{
				bCanAttack = false;
				bAttackSaved = true;
				switch (AttackCombo)
				{
				case 0:
					bAttack1 = true;
					bAttack3 = false;
					AttackCombo++;
					break;
				case 1:
					bAttack2 = true;
					bAttack1 = false;
					AttackCombo++;
					break;
				case 2:
					bAttack3 = true;
					bAttack2 = false;
					AttackCombo = 0;
					AdjustSpeed(0.0f);
					break;
				}
			}
			break;
		case EAttackType::Ranged:
			if (bCanAttack)
			{
				bCanAttack = false;
				bFireArrow = true;
				bHeld = true;
			}
			break;
		}
	}
}

void ARPGPlayer::CancelAttack()
{
	if (bHeld)
	{
		bHeld = false;
	}
}

void ARPGPlayer::AdjustSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ARPGPlayer::ResetAttack()
{
	bAttackSaved = false;
	AttackCombo = 0;
	bAttack1 = false;
	bAttack2 = false;
	bAttack3 = false;
	AdjustSpeed(450.0f);

	FTimerHandle DelayHandle;
	float DelayTime = 0.1; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]()
		{
			bCanAttack = true;

		}), DelayTime, false);
}

void ARPGPlayer::AttackTrace()
{
	FVector StartLoc = AttackFrom->GetComponentLocation();
	FVector EndLoc = StartLoc + AttackFrom->GetForwardVector() * 150;
	FHitResult Hit;
	FCollisionQueryParams Params;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	IgnoreActors.Add(SwordRef);
	IgnoreActors.Add(ShieldRef);
	Params.AddIgnoredActors(IgnoreActors);

	if (GetWorld()->SweepSingleByChannel(Hit, StartLoc, EndLoc, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(20.0f), Params))
	{
		if (Hit.GetActor()->ActorHasTag(TEXT("Enemy")))
		{
			int32 DamageMin = (Stat->Strength + SwordModifier) - FMath::RandRange(2,5);
			int32 DamageMax = Stat->Strength + SwordModifier;
			float DamageAmount = FMath::RandRange(DamageMin, DamageMax);
			FDamageEvent DamageEvent;

			Hit.GetActor()->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f"), DamageAmount));
		}

		int32 DamageMin = (Stat->Strength + SwordModifier) - FMath::RandRange(2, 5);
		int32 DamageMax = Stat->Strength + SwordModifier;
		float DamageAmount = FMath::RandRange(DamageMin, DamageMax);
		FDamageEvent DamageEvent;
		Hit.GetActor()->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f"), DamageAmount));
	}

	DrawDebugSphere(GetWorld(), EndLoc, 30.0f, 10, FColor::Cyan, false, 1, 0, 1);
}

void ARPGPlayer::FiringArrow()
{
	ArrowAimCheck();
	
	FVector SpawnLoc;
	FRotator SpawnRot;
	SpawnLoc = FireFrom->GetComponentTransform().GetLocation();
	SpawnRot = UKismetMathLibrary::FindLookAtRotation(SpawnLoc, ArrowTarget);

	ArrowRef = GetWorld()->SpawnActor<AInteractionBase>(ArrowClass, SpawnLoc, SpawnRot);

	ArrowRef->Sphere->SetCollisionProfileName(TEXT("NoCollision"));

	//FVector Loc = FireFrom->GetComponentTransform().GetLocation();;
	//FRotator Rot = FollowCamera->GetRelativeRotation();
	//FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(Loc, Rot);

	//ArrowRef = GetWorld()->SpawnActor<AInteractionBase>(ArrowClass, SpawnTransform);
}

FVector ARPGPlayer::ArrowAimCheck()
{
	FVector StartLoc = FollowCamera->GetComponentLocation();
	FVector EndLoc = StartLoc + GetViewRotation().Vector() * 15000;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECC_Visibility, Params))
	{
		ArrowTarget = Hit.Location;
	}
	else
	{
		ArrowTarget = EndLoc;
	}

	return ArrowTarget;
}







