// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NBGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/NBPlayerController.h"
#include "Net/UnrealNetwork.h"

void ANBGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ANBPlayerController* NBPC = Cast<ANBPlayerController>(PC);
			if (IsValid(NBPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				NBPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

void ANBGameStateBase::OnRep_RemainingTime()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) == true)
	{
		ANBPlayerController* NBPC = Cast<ANBPlayerController>(PC);
		if (IsValid(NBPC) == true)
		{
			NBPC->UpdateTimeUI(RemainingTime);
		}
	}
}

void ANBGameStateBase::OnRep_CurrentPlayer()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) == true)
	{
		ANBPlayerController* NBPC = Cast<ANBPlayerController>(PC);
		if (IsValid(NBPC) == true)
		{
			NBPC->UpdatePlayerUI(PlayerList, CurrentPlayer);
		}
	}
}

void ANBGameStateBase::OnRep_PlayerList()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) == true)
	{
		ANBPlayerController* NBPC = Cast<ANBPlayerController>(PC);
		if (IsValid(NBPC) == true)
		{
			NBPC->UpdatePlayerUI(PlayerList, CurrentPlayer);
		}
	}
}

void ANBGameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, CurrentPlayer);
	DOREPLIFETIME(ThisClass, PlayerList);
}
