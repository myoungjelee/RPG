
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

	FItemInfo() : ItemClass(nullptr), ItemName(FText::FromString(TEXT("None"))), ItemImage(nullptr), CurrentStack(0), MaxStack(0), ItemIndex(0), Potency(0.0f), Price(0), SellValue(0)
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> ItemImageRef(TEXT("/Script/Engine.Texture2D'/Game/InfinityBladeGrassLands/Effects/FX_Textures/Tile/T_Black_32.T_Black_32'"));
		if (ItemImageRef.Object)
		{
			ItemImage = ItemImageRef.Object;
		}
	}
	
	

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


	// 두 FItemInfo 객체의 이름과 유형이 동일하면 true를 반환(이걸 해줘야 아이템끼리 같은지 같지 않은지 비교가능)
	bool operator==(const FItemInfo& Other) const
	{
		return (ItemName.EqualTo(Other.ItemName)) && (ItemType == Other.ItemType);
	}
};