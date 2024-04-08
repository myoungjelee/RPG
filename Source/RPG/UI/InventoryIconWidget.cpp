// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryIconWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/RPGPlayer.h"
#include "Kismet/GameplayStatics.h"


void UInventoryIconWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//ItemQuantity->VisibilityDelegate.BindUFunction(this, TEXT("SetItemQuantityVisible"));
}

void UInventoryIconWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemQuantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemQuantity")));

	// Btn ��Ÿ�� ����
	FButtonStyle WidgetStyle;
	WidgetStyle.Normal.SetResourceObject(ItemInfo.ItemImage.Get());
	WidgetStyle.Normal.SetImageSize(FVector2D(32.f, 32.f));
	WidgetStyle.Pressed = WidgetStyle.Normal;
	WidgetStyle.Hovered = WidgetStyle.Normal;
	WidgetStyle.Hovered.TintColor = FLinearColor::Gray;

	Btn->SetStyle(WidgetStyle);

	// Btn Ŭ������
	Btn->OnClicked.AddDynamic(this, &UInventoryIconWidget::BtnClick);

	// ������ �̸�, ���� ����
	ItemName->SetText(ItemInfo.ItemName);
	ItemQuantity->SetText(FText::FromString(FString::FromInt(ItemInfo.CurrentStack)));
	SetItemQuantityVisible();
}

//void UInventoryIconWidget::SetItemInfo(const FItemInfo& NewItemInfo)
//{
//	ItemInfo = NewItemInfo;
//}

void UInventoryIconWidget::SetItemQuantityVisible()
{
	if (ItemInfo.CurrentStack > 0)
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventoryIconWidget::BtnClick()
{
	ARPGPlayer* Player = Cast<ARPGPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player->ItemSelected == ItemInfo)
	{
		Player->UseItem(Player->ItemSelected);
	}
	else
	{
		Player->ItemSelected = ItemInfo;
	}
}









