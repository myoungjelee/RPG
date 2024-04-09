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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class ARPGPlayer> Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class UInventoryIconWidget> IconWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInventoryIconWidget> IconWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> InventoryPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Use;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Drop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Bow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Acc;

	UFUNCTION()
	void BuildIventory();

	UFUNCTION()
	void BtnUseClick();

	UFUNCTION()
	void BtnSwordClick();

	UFUNCTION()
	void BtnShieldClick();

	UFUNCTION()
	void BtnBowClick();

	UFUNCTION()
	void BtnArrowClick();

	UFUNCTION()
	void BtnAccClick();

	void CheckBtnStyle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Column;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RowSize;
};
