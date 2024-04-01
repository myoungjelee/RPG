// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuWidget.h"
#include "Player/RPGCharacter.h"
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

	Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	InventoryPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryPanel")));
	
	BuildIventory();
}

void UMenuWidget::BuildIventory()
{
	Row = 0;
	Column = 0;

	for (int32 i = 0;  i < Player->IventorySize; i++)
	{
		IconWidget = CreateWidget<UInventoryIconWidget>(GetWorld(), IconWidgetClass);
		IconWidget->SetItemInfo(Player->Inventory[i]);

		InventoryPanel->AddChildToUniformGrid(IconWidget, Row, Column);
		Column++;
		if (Column >= RowSize)
		{
			Column = 0;
			Row++;
		}
	}
}
