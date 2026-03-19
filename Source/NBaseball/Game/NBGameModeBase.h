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

	bool CheckWin(ANBPlayerController* InChattingPlayerController, int InStrikeCount);

	void CheckDraw();

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ANBPlayerController>> AllPlayerControllers;


public:
	void PassTurn(bool IsTimeOver);

protected:
	void UpdateTimer();

	FTimerHandle TimerHandle;
	int32 MaxTime = 15;
	int32 CurrentTime = 15;
	int32 CurrentTurnPlayerIndex = 0;
};
