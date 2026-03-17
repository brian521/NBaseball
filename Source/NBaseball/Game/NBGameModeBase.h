// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBGameModeBase.generated.h"

class ANBPlayerController;

/**
 * 
 */
UCLASS()
class NBASEBALL_API ANBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(ANBPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ANBPlayerController* InChattingPlayerController);

	FString CombinePlayerInfo(ANBPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void ResetGame();

	void JudgeGame(ANBPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ANBPlayerController>> AllPlayerControllers;

protected:
	void UpdateTimer();

	FTimerHandle TimerHandle;
	int32 MaxTime = 10;
	int32 CurrentTime = 10;
};
