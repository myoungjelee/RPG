// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuWidget.h"
#include "Player/RPGPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InventoryIconWidget.h"
#include "Components/UniformGridPanel.h"

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
	
	BuildIventory();
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