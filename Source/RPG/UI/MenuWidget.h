// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class ARPGCharacter> Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class UInventoryIconWidget> IconWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInventoryIconWidget> IconWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> InventoryPanel;

	UFUNCTION()
	void BuildIventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Column;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RowSize;
};
