// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "Player/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterStat/PlayerStatComponent.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	MPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPBar")));
	XPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("XPBar")));
	Level = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level")));

	Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	MaxHp = Player->Stat->MaxHp;
	MaxMp = Player->Stat->MaxMp;
	NextLevelXP = Player->Stat->NextLevelXP;
	CurrentLevel = Player->Stat->Level;

	Player->Stat->OnHpChanged.AddUObject(this, &UPlayerHUDWidget::UpdateHpBar);
	Player->Stat->OnMpChanged.AddUObject(this, &UPlayerHUDWidget::UpdateMpBar);
	Player->Stat->OnXpChanged.AddUObject(this, &UPlayerHUDWidget::UpdateXPBar);
	Player->Stat->OnLevelChanged.AddUObject(this, &UPlayerHUDWidget::UpdateLevel);

}

void UPlayerHUDWidget::UpdateHpBar(float CurrentHp)
{
	float Hp = CurrentHp / MaxHp;
	HPBar->SetPercent(Hp);
}

void UPlayerHUDWidget::UpdateMpBar(float CurrentMp)
{
	float Mp = CurrentMp / MaxMp;
	MPBar->SetPercent(Mp);
}

void UPlayerHUDWidget::UpdateXPBar(float CurrentXp)
{
	float Xp = CurrentXp / NextLevelXP;
	XPBar->SetPercent(Xp);
}

void UPlayerHUDWidget::UpdateLevel(float NewLevel)
{
	Level->SetText(FText::AsNumber(NewLevel));
}
