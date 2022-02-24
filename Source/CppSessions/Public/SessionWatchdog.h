// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SessionWatchdog.generated.h"

UCLASS()
class CPPSESSIONS_API ASessionWatchdog : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASessionWatchdog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
