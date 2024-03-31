// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/PlayerStatComponent.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Ω∫≈» √ ±‚»≠
	Level = 1;
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;
	MaxMana = 100.f;
	CurrentMana = MaxMana;
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

	// ...
	
}


// Called every frame
void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

