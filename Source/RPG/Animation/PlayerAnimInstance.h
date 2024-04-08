// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ARPGPlayer> Player;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	//TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsEquipMelee : 1;

};
