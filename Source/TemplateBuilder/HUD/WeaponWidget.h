// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TemplateBuilder/Data/CustomStructLibrary.h"
#include "WeaponWidget.generated.h"

UCLASS()
class TEMPLATEBUILDER_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void Setup();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWeaponData(const FWeaponData& PrimaryWeapon, const FWeaponData& SecondaryWeapon, const bool bPrimaryEquipped,const bool bSecondaryEquipped);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FWeaponData PrimaryWeaponData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FWeaponData SecondaryWeaponData;
	
private:

	
};
