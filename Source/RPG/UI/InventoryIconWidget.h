// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/ItemInfo.h"
#include "InventoryIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UInventoryIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized()override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> Btn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemQuantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FItemInfo ItemInfo;


public:
	void SetItemInfo(const FItemInfo& NewItemInfo);
	void SetItemQuantityVisible();
	void OnClick();
};
