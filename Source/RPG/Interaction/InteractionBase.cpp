// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/InteractionBase.h"
#include "Components/StaticMeshComponent.h"
#include "Player/RPGCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractionBase::AInteractionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Infinity_Blade_Assets/Meshes/Props/SM_TreasureBags02.SM_TreasureBags02'"));
	if (MeshRef.Object)
	{
		Mesh->SetStaticMesh(MeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AInteractionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractionBase::Interact()
{
	ARPGCharacter* Player = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->PickupItem(ItemInfo);
		Destroy();
	}
}

