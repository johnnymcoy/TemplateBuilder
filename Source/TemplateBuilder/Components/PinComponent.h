// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PinComponent.generated.h"

UENUM(BlueprintType)
enum class EPinType : uint8
{
	Location,
	Enemy,
	Loot
	//Can Add more

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBUILDER_API UPinComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPinComponent();

	UFUNCTION(BlueprintCallable, Category = "Pin")
	void AddPin();


protected:
	virtual void BeginPlay() override;

	void CreatePin(EPinType PinType);

	UPROPERTY()
	class APinBase* LocationPin;

	UPROPERTY(EditDefaultsOnly, Category = "Pin")
	TSubclassOf<class APinBase> LocationPinClass;
	UPROPERTY(EditDefaultsOnly, Category = "Pin")
	TSubclassOf<class APinBase> LootPinClass;
	UPROPERTY(EditDefaultsOnly, Category = "Pin")
	TSubclassOf<class APinBase> EnemyPinClass;


private:
	void DestroyLastPin();

	void GetTraceParams(); 
	//Linetrace
	FVector InLocation;
	FRotator InRotation;
	//Pin Spawn
	FVector SpawnLocation;
	FRotator SpawnRotation;
	AActor* HitActor;

};
