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

	// Btn 스타일 설정
	FButtonStyle WidgetStyle;
	WidgetStyle.Normal.SetResourceObject(ItemInfo.ItemImage.Get());
	WidgetStyle.Normal.SetImageSize(FVector2D(32.f, 32.f));
	WidgetStyle.Pressed = WidgetStyle.Normal;
	WidgetStyle.Hovered = WidgetStyle.Normal;
	WidgetStyle.Hovered.TintColor = FLinearColor::Gray;

	Btn->SetStyle(WidgetStyle);

	// Btn 클릭설정
	Btn->OnClicked.AddDynamic(this, &UInventoryIconWidget::BtnClick);

	// 아이템 이름, 수량 설정
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









