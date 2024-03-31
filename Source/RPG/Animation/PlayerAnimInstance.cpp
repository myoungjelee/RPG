// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Player/RPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	//Owner = Cast<ARPGCharacter>(GetOwningActor());
	
	APawn* Pawn = TryGetPawnOwner();
	Owner = Cast<ARPGCharacter>(Pawn);
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Movement)
	{
		Velocity = Movement->Velocity;
		/*Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
		Speed = Velocity.Length();*/
		Speed = Velocity.Size2D();
		bIsFalling = Movement->IsFalling();
	}
}
