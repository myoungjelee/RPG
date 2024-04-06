// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam (FOnHpChangedDelegate, float CurrentHp);
DECLARE_MULTICAST_DELEGATE_OneParam (FOnMpChangedDelegate, float CurrentMp);
DECLARE_MULTICAST_DELEGATE_OneParam (FOnXpChangedDelegate, float CurrentXp);
DECLARE_MULTICAST_DELEGATE_OneParam (FOnLevelChangedDelegate, float CurrentLevel);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
public:

// Ω∫≈» º≥¡§
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float CurrentMp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float MaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float CurrentXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	float NextLevelXP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int32 Strength		;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipments, meta = (AllowPrivateAccess = "true"))
	int32 SwordModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipments, meta = (AllowPrivateAccess = "true"))
	int32 ShieldModifier;

	FOnHpChangedDelegate OnHpChanged;
	FOnMpChangedDelegate OnMpChanged;
	FOnXpChangedDelegate OnXpChanged;
	FOnLevelChangedDelegate OnLevelChanged;

	void SetHp(float NewHp);
	void SetMp(float NewMp);
	void SetXp(float NewXp);
	void SetLevel(float NewLevel);

};
