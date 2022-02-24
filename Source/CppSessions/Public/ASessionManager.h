// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>
#include <CoreMinimal.h>

#include "ASessionManager.generated.h"
/**
 * 
 */
UCLASS()
class CPPSESSIONS_API ASessionManager : public AActor
{
	GENERATED_BODY()
	
public:
	ASessionManager();

protected:
	virtual void BeginPlay() override;
	// virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void _handleCreateSessionComplete(bool Successful);
	UFUNCTION()
	void _handleStartSessionComplete(bool Successful);
	UFUNCTION()
	void _handleDestroySessionComplete(bool Successful);

	bool _userIsHost;
	bool _userInSession;
};
