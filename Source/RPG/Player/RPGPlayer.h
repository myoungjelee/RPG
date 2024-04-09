// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/InteractionInterface.h"
#include "Interaction/ItemInfo.h"
#include "RPGPlayer.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None,
	Melee,
	Ranged
};

UCLASS()
class RPG_API ARPGPlayer : public ACharacter
{
	GENERATED_BODY()



public:
	//================================================================================================================
	//  PROPERTIES & VARIABLES
	//================================================================================================================

	// 캐릭터 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> InteractionRadius;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> AttackFrom;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> FireFrom;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerStatComponent> Stat;

	// 입력 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DrawSwordAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DrawBowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;
	
	// 애니메이션 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
	TSubclassOf<class UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	uint8 bIsChangingEquipment : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	uint8 bSwordDrawn : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	uint8 bBowDrawn : 1;

	// HUD UI 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPlayerHUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPlayerHUDWidget> HUDWidget;

	// 인터랙션 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	TArray<AInteractionBase*> InteractableActors;

	// Inventory 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemInfo> ItemListInInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bMenuOpen : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMenuWidget> MenuWidget;

	// 장비 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AInteractionBase> SwordRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AInteractionBase> ShieldRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AInteractionBase> BowRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AInteractionBase> ArrowRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AInteractionBase> AccRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ItemSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo SwordInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SwordModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ShieldInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShieldModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo AccInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo BowInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ArrowInfo;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bItemEquip : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint8 bSwordEquip : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint8 bShieldEquip : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint8 bBowEquip : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint8 bArrowEquip : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint8 bAccEquip : 1;

	// 공격 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCanAttack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bAttackSaved : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackCombo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bAttack1 : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bAttack2 : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bAttack3 : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bFireArrow : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCancel : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ArrowTarget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AInteractionBase> ArrowClass;

	//================================================================================================================
	//  FUNCTIONS
	//================================================================================================================

	ARPGPlayer();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	// Interaction 설정
	void PickupItem(const FItemInfo& PickupItemInfo);
	
	UFUNCTION()
	void InteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void InteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Interaction();

	void ToggleMenu();
	void OpenMenu();
	void CloseMenu();

	// 장비
	UFUNCTION(BlueprintCallable)
	void UseItem(const FItemInfo& ItemInfo);
	void RemoveItem(const FItemInfo& ItemInfo);
	void RestoreHP(float RestoreAmount);
	void RestoreMP(float RestoreAmount);
	void ChangeSword(const FItemInfo& ItemInfo);
	void ChangeShield(const FItemInfo& ItemInfo);
	void ChangeAcc(const FItemInfo& ItemInfo);
	void EquipBow(const FItemInfo& ItemInfo);

	void DrawSword();
	void DrawBow();

	//아이템 장착
	UFUNCTION(BlueprintCallable)
	void SpawnGear();

	UFUNCTION(BlueprintCallable)
	void DestroyGear();	

	// 카메라 이동 설정
	void MeleeCamShift();
	void MeleeCamReShift();
	void BowCamShift();
	void BowCamReShift();

	// 공격 설정
	void Attack();
	void CancelAttack();
	void AdjustSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	UFUNCTION(BlueprintCallable)
	void AttackTrace();

	UFUNCTION(BlueprintCallable)
	void FiringArrow();

	FVector ArrowAimCheck();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Jump();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
	
};
