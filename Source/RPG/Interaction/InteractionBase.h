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
	Dagger,
	Sword,
	WoodShield UMETA(DisplayName = "WoodShield"),
	IronShield UMETA(DisplayName = "IronShield"),
	WoodBow UMETA(DisplayName = "WoodBow"),
	WoodArrows UMETA(DisplayName = "WoodArrows")
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
	TObjectPtr<class USphereComponent> Sphere;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	//TObjectPtr<class UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionBase | AItemSettings")
	EItemNames ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionBase | AItemSettings")
	FItemInfo ItemInfo;

};
