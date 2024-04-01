// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryIconWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UInventoryIconWidget::NativeConstruct()
{
	Super::NativeConstruct();


	// Btn ��Ÿ�� ����
	FButtonStyle WidgetStyle;
	WidgetStyle.Normal.SetResourceObject(ItemInfo.ItemImage.Get());
	WidgetStyle.Normal.SetImageSize(FVector2D(32.f, 32.f));
	WidgetStyle.Pressed = WidgetStyle.Normal;
	WidgetStyle.Hovered = WidgetStyle.Normal;
	WidgetStyle.Hovered.TintColor = FLinearColor::Gray;

	Btn->SetStyle(WidgetStyle);

	// ������ �̸�, ���� ����
	ItemName->SetText(ItemInfo.ItemName);
	ItemQuantity->SetText(FText::FromString(FString::FromInt(ItemInfo.CurrentStack)));
}

void UInventoryIconWidget::SetItemInfo(const FItemInfo& NewItemInfo)
{
	ItemInfo = NewItemInfo;
}









