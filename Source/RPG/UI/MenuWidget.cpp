// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuWidget.h"
#include "Player/RPGPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InventoryIconWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Interaction/InteractionBase.h"

UMenuWidget::UMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RowSize = 5;

	ConstructorHelpers::FClassFinder<UInventoryIconWidget> IconRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/Widget/WBP_IventoryIcon.WBP_IventoryIcon_C'"));
	if (IconRef.Class)
	{
		IconWidgetClass = IconRef.Class;
	}
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<ARPGPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	InventoryPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryPanel")));

	Btn_Use->OnClicked.AddDynamic(this, &UMenuWidget::BtnUseClick);
	Btn_Sword->OnClicked.AddDynamic(this, &UMenuWidget::BtnSwordClick);
	Btn_Shield->OnClicked.AddDynamic(this, &UMenuWidget::BtnShieldClick);
	Btn_Accessory->OnClicked.AddDynamic(this, &UMenuWidget::BtnAccClick);
	
	BuildIventory();
	CheckGear();
}

//void UMenuWidget::BuildIventory()
//{
//	Row = 0;
//	Column = 0;
//
//
//	for (int32 i = 0;  i < Player->InventorySize; i++)
//	{
//		IconWidget = CreateWidget<UInventoryIconWidget>(GetWorld(), IconWidgetClass);
//		if (Player->ItemListInInventory.Num() > 0)
//		{
//			IconWidget->ItemInfo = Player->ItemListInInventory[i];
//		}
//
//		InventoryPanel->AddChildToUniformGrid(IconWidget, Row, Column);
//
//		Column++;
//		if (Column >= RowSize)
//		{
//			Column = 0;
//			Row++;
//		}
//	}
//}


void UMenuWidget::BuildIventory()
{
	Row = 0;
	Column = 0;

	int32 NumItems = Player->ItemListInInventory.Num(); // 인벤토리 아이템 리스트의 크기를 저장

	for (int32 i = 0; i < Player->InventorySize; i++)
	{
		// 배열 인덱스가 유효한지 확인
		if (i < NumItems)
		{
			IconWidget = CreateWidget<UInventoryIconWidget>(GetWorld(), IconWidgetClass);
			IconWidget->ItemInfo = Player->ItemListInInventory[i];
		}
		else
		{
			// 인벤토리 아이템이 없을 때 처리
			IconWidget = CreateWidget<UInventoryIconWidget>(GetWorld(), IconWidgetClass);
			// 빈 아이템 정보 또는 다른 처리
		}

		InventoryPanel->AddChildToUniformGrid(IconWidget, Row, Column);

		Column++;
		if (Column >= RowSize)
		{
			Column = 0;
			Row++;
		}
	}
}

void UMenuWidget::BtnUseClick()
{
	Player->UseItem(Player->ItemSelected);
}

void UMenuWidget::BtnSwordClick()
{
	if (Player->SwordInfo.ItemClass)
	{
		Player->PickupItem(Player->SwordInfo);
		Player->SwordInfo = FItemInfo();
		CheckGear();
	}
}

void UMenuWidget::BtnShieldClick()
{
	if (Player->ShieldInfo.ItemClass)
	{
		Player->PickupItem(Player->ShieldInfo);
		Player->ShieldInfo = FItemInfo();
		CheckGear();
	}
}

void UMenuWidget::BtnAccClick()
{
	if (Player->AccessoryInfo.ItemClass)
	{
		Player->PickupItem(Player->AccessoryInfo);
		Player->AccessoryInfo = FItemInfo();
		CheckGear();
	}
}

void UMenuWidget::CheckGear()
{
	FButtonStyle SwordBtnStyle;

	SwordBtnStyle.Normal.SetResourceObject(Player->SwordInfo.ItemImage);
	SwordBtnStyle.Pressed.SetResourceObject(Player->SwordInfo.ItemImage);
	SwordBtnStyle.Hovered.SetResourceObject(Player->SwordInfo.ItemImage);
	SwordBtnStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));

	Btn_Sword->SetStyle(SwordBtnStyle);

	FButtonStyle ShieldBtnStyle;

	ShieldBtnStyle.Normal.SetResourceObject(Player->ShieldInfo.ItemImage);
	ShieldBtnStyle.Pressed.SetResourceObject(Player->ShieldInfo.ItemImage);
	ShieldBtnStyle.Hovered.SetResourceObject(Player->ShieldInfo.ItemImage);
	ShieldBtnStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));

	Btn_Shield->SetStyle(ShieldBtnStyle);

	//FButtonStyle ArrowBtnStyle;

	//ArrowBtnStyle.Normal.SetResourceObject(Player->ArrowdInfo.ItemImage);
	//ArrowBtnStyle.Pressed.SetResourceObject(Player->ArrowdInfo.ItemImage);
	//ArrowBtnStyle.Hovered.SetResourceObject(Player->ArrowdInfo.ItemImage);
	//ArrowBtnStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));

	//Btn_Arrow->SetStyle(ArrowBtnStyle);

	//FButtonStyle BowBtnStyle;

	//BowBtnStyle.Normal.SetResourceObject(Player->BowInfo.ItemImage);
	//BowBtnStyle.Pressed.SetResourceObject(Player->BowInfo.ItemImage);
	//BowBtnStyle.Hovered.SetResourceObject(Player->BowInfo.ItemImage);
	//BowBtnStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));

	//Btn_Bow->SetStyle(BowBtnStyle);

	FButtonStyle AccessoryBtnStyle;

	AccessoryBtnStyle.Normal.SetResourceObject(Player->AccessoryInfo.ItemImage);
	AccessoryBtnStyle.Pressed.SetResourceObject(Player->AccessoryInfo.ItemImage);
	AccessoryBtnStyle.Hovered.SetResourceObject(Player->AccessoryInfo.ItemImage);
	AccessoryBtnStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));

	Btn_Accessory->SetStyle(AccessoryBtnStyle);
}
