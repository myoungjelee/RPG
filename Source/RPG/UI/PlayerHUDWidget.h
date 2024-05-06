// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> MPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> XPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Level;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> CrossHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<class ARPGPlayer> Player;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<class UTexture2D> ItemImageTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FText ItemNameText;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float MaxMp;

	UPROPERTY()
	float NextLevelXP;

	UPROPERTY()
	int32 CurrentLevel;

public:
	//void UpdateHpBar(float CurrentHp);
	//void UpdateMpBar(float CurrentMp);
	//void UpdateXPBar(float CurrentXp);
	//void UpdateLevel(float NewLevel);
};

