// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/PlayerStatComponent.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 스탯 초기화
	Level = 1;
	MaxHp = 100.f;
	CurrentHp = MaxHp;
	MaxMp = 100.f;
	CurrentMp = MaxMp;
	CurrentXP = 0.f;
	NextLevelXP = 50.f;
	Strength = 10;
	Defense = 10;
	SwordModifier = 0;
	ShieldModifier = 0;
}

// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle WaitHandle;
	float WaitTime = 0.1; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			SetHp(CurrentHp);
			SetMp(CurrentMp);
			SetXp(CurrentXP);
			SetLevel(Level);
		}), WaitTime, false);

}


// Called every frame
void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0, MaxHp);
	//OnHpChanged.Broadcast(CurrentHp);
}

void UPlayerStatComponent::SetMp(float NewMp)
{
	CurrentMp = FMath::Clamp(NewMp, 0, MaxMp);
	//OnMpChanged.Broadcast(CurrentMp);
}

void UPlayerStatComponent::SetXp(float NewXp)
{
	CurrentXP = FMath::Clamp(NewXp, 0, NextLevelXP);
	//OnXpChanged.Broadcast(CurrentXP);
}

void UPlayerStatComponent::SetLevel(float NewLevel)
{
	//OnLevelChanged.Broadcast(NewLevel);
}

