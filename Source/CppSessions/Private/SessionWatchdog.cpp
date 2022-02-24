// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionWatchdog.h"

#include "Subsystems/CSSessionSubsystem.h"

// Sets default values
ASessionWatchdog::ASessionWatchdog()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASessionWatchdog::BeginPlay()
{
	Super::BeginPlay();
}

void ASessionWatchdog::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	auto sessionSubsystem =
		GetWorld()->GetGameInstance()->GetSubsystem<UCSSessionSubsystem>();
	if (GetLocalRole() == ROLE_Authority)
	{
		// If you are the hosting player, destroy the session
		UE_LOG(LogTemp, Display, TEXT("Destroying session..."));
		sessionSubsystem->DestroySession();
	}
	Super::EndPlay(EndPlayReason);
}
