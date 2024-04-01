// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/ItemInfo.h"
#include "Interface/InteractionInterface.h"
#include "InteractionBase.generated.h"

//���� ����
UENUM(BlueprintType)//����� �������� ���Ʈ��Ʈ���� ���������
enum class EItemNames : uint8
{
	HPPotion,
	MPPotion,
	RustedSword,
	IronSword,
	WoodSheild,
	IronShieild
};


UCLASS()
class RPG_API AInteractionBase : public AActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemNames ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemInfo ItemInfo;
};
