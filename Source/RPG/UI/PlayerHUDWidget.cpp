// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUDWidget.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "Player/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	MPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ManaBar")));
	XPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("XPBar")));
	Level = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level")));

	Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

//void UPlayerHUDWidget::UpdateHpBar(float CurrentHp, float MaxHp)
//{
//	float Hp = CurrentHp / MaxHp;
//	HPBar->SetPercent(Hp);
//}
//
//void UPlayerHUDWidget::UpdateMpBar(float CurrentMp, float MaxMp)
//{
//	float Mp = CurrentMp / MaxMp;
//	MPBar->SetPercent(Mp);
//}
//
//void UPlayerHUDWidget::UpdateXPBar(float CurrentXp, float NextXp)
//{
//	float Xp = CurrentXp / NextXp;
//	XPBar->SetPercent(Xp);
//}
