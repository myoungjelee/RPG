// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
public:

// Ω∫≈» º≥¡§
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float CurrentXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float NextLevelXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int32 Strength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipments, meta = (AllowPrivateAccess = "true"))
	int32 SwordModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipments, meta = (AllowPrivateAccess = "true"))
	int32 ShieldModifier;
		
};
