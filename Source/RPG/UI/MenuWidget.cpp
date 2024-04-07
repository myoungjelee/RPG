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

	int32 NumItems = Player->ItemListInInventory.Num(); // �κ��丮 ������ ����Ʈ�� ũ�⸦ ����

	for (int32 i = 0; i < Player->InventorySize; i++)
	{
		// �迭 �ε����� ��ȿ���� Ȯ��
		if (i < NumItems)
		{
			IconWidget = CreateWidget<UInventoryIconWidget>(GetWorld(), IconWidgetClass);
			IconWidget->ItemInfo = Player->ItemListInInventory[i];
		}
		else
		{
			// �κ��丮 �������� ���� �� ó��
			IconWidget = CreateWidget<UInventoryIconWidget>(GetWorld(), IconWidgetClass);
			// �� ������ ���� �Ǵ� �ٸ� ó��
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