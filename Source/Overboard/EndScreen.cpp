// Fill out your copyright notice in the Description page of Project Settings.


#include "EndScreen.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "CustomSaveGame.h"
#include "ScreenLogger.h"

void UEndScreen::NativeConstruct()
{
    _saveButton->OnClicked.AddDynamic(this, &UEndScreen::OnSaveClicked);
    _cancelButton->OnClicked.AddDynamic(this, &UEndScreen::OnCancelClicked);
    _restartButton->OnClicked.AddDynamic(this, &UEndScreen::OnRestartClicked);
    _quitButton->OnClicked.AddDynamic(this, &UEndScreen::OnQuitClicked);

    // Show entry UI first, hide leaderboard
    _leaderboardPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UEndScreen::OnSaveClicked()
{
    const FString lName = _initialsTextBox->GetText().ToString();
	_scoreDatas.Add(FScoreData(lName, _score));
    SaveScoreToFile();
    ShowLeaderboard();
}
void UEndScreen::Init(int pScore)
{
	_score = pScore;
	_saveButton->SetIsEnabled(false);
	FName lName = FName(*FString::FromInt(pScore));
	_initialsTextBox->OnTextChanged.AddDynamic(this,&UEndScreen::OnInitialTextChanged);
    LoadScores();
}

void UEndScreen::OnInitialTextChanged(const FText& pText)
{
	int lTextLength = pText.ToString().Len();

	if (lTextLength > 3)
	{
		_initialsTextBox->SetText(FText::FromString(pText.ToString().Left(3).ToUpper()));
	    _saveButton->SetIsEnabled(true);
	}
    else
    {
        _initialsTextBox->SetText(FText::FromString(pText.ToString().ToUpper()));
		_saveButton->SetIsEnabled(lTextLength == 3);
	}
}

void UEndScreen::OnCancelClicked()
{
    ShowLeaderboard();
}

void UEndScreen::SaveScoreToFile()
{
	UScreenLogger::WriteInfo("Saving score...");
    // Load or create SaveGame
    UCustomSaveGame* SaveGameInstance;

    if (UGameplayStatics::DoesSaveGameExist(SAVE_KEY, 0))
    {
        SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_KEY, 0));
        UScreenLogger::WriteInfo("Save exist");
    }
    else
    {
        SaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));
        UScreenLogger::WriteInfo("Save Not exist");
    }

    if (SaveGameInstance)
    {
        UScreenLogger::WriteInfo("Save");
		UScreenLogger::WriteOnScreen(_scoreDatas.Num());
        SaveGameInstance->SavedScores = _scoreDatas;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVE_KEY, 0);
    }
}

void UEndScreen::LoadScores()
{
    _scoreDatas.Empty();
    UScreenLogger::WriteInfo("Load");
    if (UGameplayStatics::DoesSaveGameExist(SAVE_KEY, 0))
    {
        UScreenLogger::WriteInfo("Save exist for load");
        UCustomSaveGame* SaveGameInstance = Cast<UCustomSaveGame>(
            UGameplayStatics::LoadGameFromSlot(SAVE_KEY, 0));

        if (SaveGameInstance)
        {
            UScreenLogger::WriteInfo("Save instance found");
            _scoreDatas = SaveGameInstance->SavedScores;
            UScreenLogger::WriteOnScreen(_scoreDatas.Num());
        }
    }
}

void UEndScreen::ShowLeaderboard()
{
    _leaderboardPanel->SetVisibility(ESlateVisibility::Visible);
    _initialsTextBox->SetVisibility(ESlateVisibility::Collapsed);
    _saveButton->SetVisibility(ESlateVisibility::Collapsed);
    _cancelButton->SetVisibility(ESlateVisibility::Collapsed);

    _scoreScrollBox->ClearChildren();

    for (const FScoreData& lData : _scoreDatas)
    {
        UTextBlock* ScoreLine = NewObject<UTextBlock>(this);
		ScoreLine->SetColorAndOpacity(FLinearColor::White);
		ScoreLine->SetJustification(ETextJustify::Center);
		ScoreLine->Font.Size = 50;
        ScoreLine->SetText(FText::FromString(lData.ToString()));
        _scoreScrollBox->AddChild(ScoreLine);
    }
}

void UEndScreen::OnRestartClicked()
{
    UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void UEndScreen::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}