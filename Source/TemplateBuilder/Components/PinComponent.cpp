// Fill out your copyright notice in the Description page of Project Settings.


#include "PinComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TemplateBuilder/Characters/ALSCustomCharacter.h"
#include "TemplateBuilder/Components/PinBase.h"
#include "TemplateBuilder/Interactable/InteractableInterface.h"

// Sets default values for this component's properties
UPinComponent::UPinComponent()
{

}

void UPinComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningPawn = Cast<APawn>(GetOwner());
	OwningPlayerController = Cast<APlayerController>(OwningPawn->GetController());
}

void UPinComponent::AddPin(FHitResult& out_Hit) 
{
	GetTraceParams();
	FVector TraceStart = InLocation;
	FVector ForwardVector = (FRotationMatrix(InRotation).GetScaledAxis( EAxis::X ));
	FVector TraceEnd = (ForwardVector * 10000.0f) + InLocation;
	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	if(UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility),
	 true, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
	{
		out_Hit = Hit;
		SpawnLocation = Hit.Location;
		FHitResult SphereHit;
		if(UKismetSystemLibrary::SphereTraceSingle(this, SpawnLocation, SpawnLocation, 50.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), 
		 true, ActorsToIgnore, EDrawDebugTrace::ForDuration, SphereHit, true, FLinearColor::Green, FLinearColor::Blue, 1.0f))
		{
			HitActor = SphereHit.Actor.Get();
			SpawnRotation = GetOwner()->GetActorRotation();
			ACharacter* CharacterRef = Cast<ACharacter>(HitActor);
			IInteractableInterface* InterfaceRef = Cast<IInteractableInterface>(HitActor);
			if(CharacterRef)
			{
				CreatePin(EPinType::Enemy);
			}
			else if(InterfaceRef)
			{
				CreatePin(EPinType::Loot);
			}
			else
			{
				DestroyLastPin();
				CreatePin(EPinType::Location);
			}
		}
	}
}

void UPinComponent::CreatePin(EPinType PinType) 
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if(PinType == EPinType::Enemy)
	{
		ACharacter* CharacterRef = Cast<ACharacter>(HitActor);
		FVector EnemyPinSpawn = CharacterRef->GetMesh()->GetSocketLocation("head");
		if(IsValid(EnemyPinClass))
		{
			APinBase* EnemyPin = GetWorld()->SpawnActor<APinBase>(EnemyPinClass, EnemyPinSpawn, SpawnRotation, SpawnParams);
			EnemyPin->AttachToActor(HitActor, FAttachmentTransformRules::KeepWorldTransform, "head");
			EnemyPin->SetOwner(HitActor);
		}
		else
		{UE_LOG(LogTemp,Error,TEXT("No Enemy Class set in PinComponent"));}
	}
	else if(PinType == EPinType::Loot)
	{
		if(IsValid(LootPinClass))
		{
			APinBase* LootPin = GetWorld()->SpawnActor<APinBase>(LootPinClass, SpawnLocation, SpawnRotation, SpawnParams);
		}
		else{UE_LOG(LogTemp,Error,TEXT("No Loot Class set in PinComponent"));}
	}
	else if(PinType == EPinType::Location)
	{
		if(IsValid(LocationPinClass))
		{
			LocationPin = GetWorld()->SpawnActor<APinBase>(LocationPinClass, SpawnLocation, SpawnRotation, SpawnParams);
		}
		else{UE_LOG(LogTemp,Error,TEXT("No Location Class set in PinComponent"));}
	}
}


void UPinComponent::DestroyLastPin() 
{
	if(IsValid(LocationPin))
	{
		LocationPin->Destroy();
	}
	//If(IsValid(LastLocationPin))
	//LastLocationPin->Destroy;
}

void UPinComponent::GetTraceParams()
{
	if(OwningPlayerController)
	{
		APlayerCameraManager* PlayerCamera = OwningPlayerController->PlayerCameraManager;
		InLocation = PlayerCamera->GetCameraLocation();
		InRotation = PlayerCamera->GetCameraRotation();
	}
}
