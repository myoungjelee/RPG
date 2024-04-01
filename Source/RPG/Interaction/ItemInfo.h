
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

UENUM(BlueprintType)//����� �������� ���Ʈ��Ʈ���� ���������
enum class EItemType : uint8
{
	None,
	HPItem,
	MPItem,
	Sword,
	Shield,
	KeyItem,
	Resources
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	FItemInfo() : CurrentStack(0), MaxStack(0), ItemIndex(0), Potency(0.0f), Price(0), SellValue(0) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AInteractionBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText  ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Potency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SellValue;
};