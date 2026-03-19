// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBPlayerController.h"

#include "UI/NBChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NBaseball.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/NBGameModeBase.h"
#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

ANBPlayerController::ANBPlayerController()
{
	bReplicates = true;
}

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UNBChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(TimerTextWidgetClass) == true)
	{
		TimerTextWidgetInstance = CreateWidget<UUserWidget>(this, TimerTextWidgetClass);
		if (IsValid(TimerTextWidgetInstance) == true)
		{
			TimerTextWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(PlayerTextWidgetClass) == true)
	{
		PlayerTextWidgetInstance = CreateWidget<UUserWidget>(this, PlayerTextWidgetClass);
		if (IsValid(PlayerTextWidgetInstance) == true)
		{
			PlayerTextWidgetInstance->AddToViewport();

			this->PlayerListBox = Cast<UVerticalBox>(PlayerTextWidgetInstance->GetWidgetFromName(TEXT("PlayerListBox")));
		}
	}

	if (IsValid(ChattingTextWidgetClass) == true)
	{
		ChattingTextWidgetInstance = CreateWidget<UUserWidget>(this, ChattingTextWidgetClass);
		if (IsValid(ChattingTextWidgetInstance) == true)
		{
			ChattingTextWidgetInstance->AddToViewport(10);

			this->ChattingBox = Cast<UVerticalBox>(ChattingTextWidgetInstance->GetWidgetFromName(TEXT("ChattingBox")));
		}
	}
}

void ANBPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ANBPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	if (!ChattingBox) return;

	UTextBlock* NewChat = NewObject<UTextBlock>(this);
	if (NewChat)
	{
		NewChat->SetText(FText::FromString(InChatMessageString));

		FSlateFontInfo FontInfo = NewChat->GetFont();
		FontInfo.Size = 25;
		FontInfo.OutlineSettings.OutlineSize = 2.f;
		NewChat->SetFont(FontInfo);

		UVerticalBoxSlot* NewSlot = ChattingBox->AddChildToVerticalBox(NewChat);

		if (NewSlot)
		{
			FMargin NewPadding(10.f);
			NewSlot->SetPadding(NewPadding);
			NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		}

		if (ChattingBox->GetChildrenCount() > 10)
		{
			ChattingBox->RemoveChildAt(0);
		}
	}
}

void ANBPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ANBPlayerController::Client_StartNotificationTimer_Implementation(float Time)
{
	GetWorldTimerManager().ClearTimer(NotificationTimerHandle);
	GetWorldTimerManager().SetTimer(NotificationTimerHandle, this, &ANBPlayerController::ClearNotificationText, Time, false);
}

void ANBPlayerController::ClearNotificationText()
{
	if (NotificationTextWidgetInstance)
	{
		NotificationText = FText::FromString(TEXT(""));
	}
}

void ANBPlayerController::UpdateTimeUI(int32 Time)
{
	if (TimerTextWidgetInstance)
	{
		TimerText = FText::Format(FText::FromString(TEXT("Time: {0}")), Time);
	}
}

void ANBPlayerController::UpdatePlayerUI(TArray<FString> PlayerList, int32 PlayerIndex)
{
	if (PlayerTextWidgetInstance)
	{
		if (PlayerListBox)
		{
			PlayerListBox->ClearChildren();
			if (PlayerList.IsEmpty()) return;
			for (int32 i = 0; i < PlayerList.Num(); ++i)
			{
				UTextBlock* NewTextBlock = NewObject<UTextBlock>(this);
				if (NewTextBlock)
				{
					FText PlayerNameText = FText::FromString(*PlayerList[i]);
					NewTextBlock->SetText(FText::Format(FText::FromString(TEXT("{0}")), PlayerNameText));
					NewTextBlock->SetJustification(ETextJustify::Right);
					if (i == PlayerIndex)
					{
						NewTextBlock->SetColorAndOpacity(FLinearColor::Red);
					}
					FSlateFontInfo FontInfo = NewTextBlock->GetFont();
					FontInfo.Size = 35;
					NewTextBlock->SetFont(FontInfo);
					UVerticalBoxSlot* NewSlot = PlayerListBox->AddChildToVerticalBox(NewTextBlock);

					if (NewSlot)
					{
						FMargin NewPadding(10.f);
						NewSlot->SetPadding(NewPadding);
						NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
					}
				}
			}
		}
	}
}

void ANBPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ANBGameModeBase* NBGM = Cast<ANBGameModeBase>(GM);
		if (IsValid(NBGM) == true)
		{
			NBGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ANBPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}