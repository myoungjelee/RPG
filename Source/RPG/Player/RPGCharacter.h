// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/InteractionInterface.h"
#include "Interaction/ItemInfo.h"
#include "RPGCharacter.generated.h"


UCLASS(config=Game)
class ARPGCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MenuAction;

public:
	ARPGCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

// 애니메이션 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	TSubclassOf<class UAnimInstance> AnimInstance;

// 스탯 설정
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerStatComponent> Stat;

// HUD UI 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPlayerHUDWidget> HUDWidget;

// Interaction 설정
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	void PickupItem(FItemInfo& PickupItemInfo);

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> InteractionRadius;

	UFUNCTION()
	void InteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void InteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TArray<AInteractionBase*> InteractableActors;

	void Interaction();

// Inventory 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemInfo> ItemListInInventory;


	void ToggleMenu();
	void OpenMenu();
	void CloseMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bMenuOpen : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMenuWidget> MenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ItemSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo SwordInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ShieldInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo AccInfo;
};

