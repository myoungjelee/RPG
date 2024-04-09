// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Player/RPGPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//Player = Cast<ARPGPlayer>(GetOwningActor());
	APawn* Pawn = TryGetPawnOwner();
	Player = Cast<ARPGPlayer>(Pawn);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
		Speed = Player->GetVelocity().Length();
		//Speed = Velocity.Size2D();
		bIsFalling = Player->GetCharacterMovement()->IsFalling();
		bIsEquipMelee = Player->bSwordDrawn;
		bAttack1 = Player->bAttack1;
		bAttack2 = Player->bAttack2;
		bAttack3 = Player->bAttack3;
		bAttackSaved = Player->bAttackSaved;	
		bDrawingBow = Player->bBowDrawn;
		bFiring = Player->bFireArrow;
		bHeld = Player->bHeld;
		bCancel = Player->bCancel;
	}
}
