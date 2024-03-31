
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

UENUM(BlueprintType)//만들어 놓은것을 블루트린트에서 쓰고싶을때
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
	int32 Potency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SellValue;
};