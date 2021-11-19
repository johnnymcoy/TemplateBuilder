// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PinBase.generated.h"

USTRUCT(BlueprintType)
struct FPinInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pin")
	UMaterialInterface* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pin")
	float Lifetime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pin")
	FVector Location;
};


UCLASS()
class TEMPLATEBUILDER_API APinBase : public AActor
{
	GENERATED_BODY()
	
public:	
	APinBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pin")
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pin")
	class UPinWidget* PinWidgetComponent;

	FPinInfo PinData;
};
