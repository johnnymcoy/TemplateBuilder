// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "HTTPFetchRequest.generated.h"

// Event that will be the 'Completed' exec wire in the blueprint node along with all parameters as output pins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHttpRequestCompleted, const FString&, Text, bool, bSuccess);

UCLASS()
class MENUSYSTEM_API UHTTPFetchRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "HTTP", WorldContext = "WorldContextObject"))
	static UHTTPFetchRequest* AsyncRequestHTTP(UObject* WorldContextObject, FString URL, FString Field);

	UPROPERTY(BlueprintAssignable)
	FOnHttpRequestCompleted OnHttpRequestCompleted;

	FString TextField;

	/* URL to send GET request to */
	FString URL;

protected:
	void HandleRequestCompleted(FString ResponseString, bool bSuccess);

};
