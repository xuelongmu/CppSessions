// Fill out your copyright notice in the Description page of Project Settings.


#include "ASessionManager.h"
#include "OnlineSessionSettings.h"
#include "Subsystems/CSSessionSubsystem.h"

ASessionManager::ASessionManager()
{
	_userIsHost = false;
	_userInSession = false;
}

void ASessionManager::BeginPlay()
{
	Super::BeginPlay();
	auto sessionSubsystem =
      GetWorld()->GetGameInstance()->GetSubsystem<UCSSessionSubsystem>();

  sessionSubsystem->OnStartSessionCompleteEvent.AddDynamic(
      this,
      &ASessionManager::_handleStartSessionComplete);
  sessionSubsystem->OnCreateSessionCompleteEvent.AddDynamic(
      this,
      &ASessionManager::_handleCreateSessionComplete);
  sessionSubsystem->OnDestroySessionCompleteEvent.AddDynamic(
      this,
      &ASessionManager::_handleDestroySessionComplete);

  // Try to find a session and join it
  UE_LOG(LogTemp, Display, TEXT("Searching for session..."));
  _userIsHost = false;
  sessionSubsystem->OnFindSessionsCompleteEvent.AddWeakLambda(
      this,
      [this, sessionSubsystem](
          const TArray<FOnlineSessionSearchResult>& SessionResults,
          bool Successful) {
        if (Successful && SessionResults.Num() > 0) {
          UE_LOG(LogTemp, Display, TEXT("Found a session. Joining..."));
          sessionSubsystem->OnJoinGameSessionCompleteEvent.AddWeakLambda(
              this,
              [this,
               sessionSubsystem](EOnJoinSessionCompleteResult::Type Result) {
                bool result = false;
                switch (Result) {
                case EOnJoinSessionCompleteResult::Type::Success:
                  UE_LOG(LogTemp, Display,TEXT("Successfully joined session!"));
                  _userInSession = true;
                  UE_LOG(LogTemp, Display,TEXT("Traveling to session..."));
                  result = sessionSubsystem->TryTravelToCurrentSession();
                  if (!result) {
                    UE_LOG(LogTemp, Display, TEXT("Could not travel to session."));
                  }
                  break;
                case EOnJoinSessionCompleteResult::Type::SessionIsFull:
                  UE_LOG(LogTemp, Display, TEXT("Error joining session: Session is full."));
                  break;
                case EOnJoinSessionCompleteResult::Type::SessionDoesNotExist:
                  UE_LOG(LogTemp, Display, 
                      TEXT("Error joining session: Session does not exist."));
                  break;
                case EOnJoinSessionCompleteResult::Type::
                    CouldNotRetrieveAddress:
                  UE_LOG(LogTemp, Display, TEXT(
                      "Error joining session: Could not retreive server address."));
                  break;
                case EOnJoinSessionCompleteResult::Type::AlreadyInSession:
                  UE_LOG(LogTemp, Display, TEXT(
                      "Error joining session: User is already in the session."));
                  break;
                case EOnJoinSessionCompleteResult::Type::UnknownError:
                  UE_LOG(LogTemp, Display, TEXT("Error joining session: Unknown error."));
                  break;
                default:
                  checkNoEntry();
                  break;
                }
              });
          sessionSubsystem->JoinGameSession(SessionResults[0]);
          return;
        }

        // If none are available, create a new session and immediately start
        UE_LOG(LogTemp, Display, TEXT("No sessions available. Creating new session..."));
        _userIsHost = true;
        sessionSubsystem->CreateSession(5, true);
      });
  sessionSubsystem->FindSessions(1, true);

}

void ASessionManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (!_userInSession) {
		return;
	}

	auto sessionSubsystem =
		GetWorld()->GetGameInstance()->GetSubsystem<UCSSessionSubsystem>();
	if (_userIsHost) {
		// If you are the hosting player, destroy the session
		UE_LOG(LogTemp, Display, TEXT("Destroying session..."));
		sessionSubsystem->DestroySession();
	}

	// Otherwise: ?

	Super::EndPlay(EndPlayReason);
}


void ASessionManager::_handleCreateSessionComplete(bool Successful) {
	if (!Successful) {
		UE_LOG(LogTemp, Display, TEXT("Unkown error while creating session."));
		return;
	}

	auto sessionSubsystem =
		GetWorld()->GetGameInstance()->GetSubsystem<UCSSessionSubsystem>();
	UE_LOG(LogTemp, Display, TEXT("Starting session..."));
	sessionSubsystem->StartSession();
}

void ASessionManager::_handleStartSessionComplete(bool Successful) {
	if (!Successful) {
		UE_LOG(LogTemp, Display, TEXT("Unknown error while starting session."));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Successfully started session!"));
	_userInSession = true;
}

void ASessionManager::_handleDestroySessionComplete(bool Successful)
{
	if (Successful) {
		UE_LOG(LogTemp, Display, TEXT("Session destroyed."));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Could not destroy session."));
}
