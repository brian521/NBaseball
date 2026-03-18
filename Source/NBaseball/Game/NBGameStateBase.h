// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class NBASEBALL_API ANBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));

	UFUNCTION()
	void OnRep_RemainingTime();

	UFUNCTION()
	void OnRep_CurrentPlayer();

	UFUNCTION()
	void OnRep_PlayerList();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, BlueprintReadOnly, Category = "Turn")
	int32 RemainingTime;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentPlayer, BlueprintReadOnly, Category = "Turn")
	int32 CurrentPlayer = 0;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerList, BlueprintReadOnly, Category = "Turn")
	TArray<FString> PlayerList;
};
