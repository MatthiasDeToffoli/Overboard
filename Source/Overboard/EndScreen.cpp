#include "EndScreen.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "CustomSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ScoreData.h"

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
    // Load or create SaveGame
    UCustomSaveGame* lSaveGameInstance;

    if (UGameplayStatics::DoesSaveGameExist(SAVE_KEY, 0))
    {
        lSaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_KEY, 0));
    }
    else
    {
        lSaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));
    }

    if (lSaveGameInstance)
    {
        lSaveGameInstance->SavedScores = _scoreDatas;

        UGameplayStatics::SaveGameToSlot(lSaveGameInstance, SAVE_KEY, 0);
    }
}

void UEndScreen::LoadScores()
{
    _scoreDatas.Empty();

    if (UGameplayStatics::DoesSaveGameExist(SAVE_KEY, 0))
    {
        UCustomSaveGame* lSaveGameInstance = Cast<UCustomSaveGame>(
            UGameplayStatics::LoadGameFromSlot(SAVE_KEY, 0));

        if (lSaveGameInstance)
        {
            _scoreDatas = lSaveGameInstance->SavedScores;
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
        UTextBlock* lScoreLine = NewObject<UTextBlock>(this);
		lScoreLine->SetColorAndOpacity(FLinearColor::White);
		lScoreLine->SetJustification(ETextJustify::Center);
		lScoreLine->Font.Size = 50;
        lScoreLine->SetText(FText::FromString(lData.ToString()));
        _scoreScrollBox->AddChild(lScoreLine);
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