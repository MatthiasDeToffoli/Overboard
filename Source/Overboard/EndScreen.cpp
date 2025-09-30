#include "EndScreen.h"
#include "Constants.h"
#include <Components/Button.h>
#include <Components/EditableTextBox.h>
#include <Components/ScrollBox.h>
#include <Components/TextBlock.h>
#include "CustomSaveGame.h"
#include <Kismet/GameplayStatics.h>
#include "ScoreData.h"

void UEndScreen::NativeConstruct()
{
    saveButton_->OnClicked.AddDynamic(this, &UEndScreen::OnSaveClicked);
    cancelButton_->OnClicked.AddDynamic(this, &UEndScreen::OnCancelClicked);
    restartButton_->OnClicked.AddDynamic(this, &UEndScreen::OnRestartClicked);
    quitButton_->OnClicked.AddDynamic(this, &UEndScreen::OnQuitClicked);

    // Show entry UI first, hide leaderboard
    leaderboardPanel_->SetVisibility(ESlateVisibility::Collapsed);
}

void UEndScreen::OnSaveClicked()
{
    const FString lName = initialsTextBox_->GetText().ToString();
	scoreDatas_.Add(FScoreData(lName, score_));
    SaveScoreToFile();
    ShowLeaderboard();
}
void UEndScreen::Init(int pScore)
{
	score_ = pScore;
	saveButton_->SetIsEnabled(false);
	FName lName = FName(*FString::FromInt(pScore));
	initialsTextBox_->OnTextChanged.AddDynamic(this,&UEndScreen::OnInitialTextChanged);
    LoadScores();
}

void UEndScreen::OnInitialTextChanged(const FText& pText)
{
	int lTextLength = pText.ToString().Len();

	if (lTextLength > 3)
	{
		initialsTextBox_->SetText(FText::FromString(pText.ToString().Left(3).ToUpper()));
	    saveButton_->SetIsEnabled(true);
	}
    else
    {
        initialsTextBox_->SetText(FText::FromString(pText.ToString().ToUpper()));
		saveButton_->SetIsEnabled(lTextLength == 3);
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

    if (UGameplayStatics::DoesSaveGameExist(Constants::Save::kSaveFileName(), 0))
    {
        lSaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::LoadGameFromSlot(Constants::Save::kSaveFileName(), 0));
    }
    else
    {
        lSaveGameInstance = Cast<UCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(UCustomSaveGame::StaticClass()));
    }

    if (lSaveGameInstance)
    {
        lSaveGameInstance->SavedScores = scoreDatas_;

        UGameplayStatics::SaveGameToSlot(lSaveGameInstance, Constants::Save::kSaveFileName(), 0);
    }
}

void UEndScreen::LoadScores()
{
    scoreDatas_.Empty();

    if (UGameplayStatics::DoesSaveGameExist(Constants::Save::kSaveFileName(), 0))
    {
        UCustomSaveGame* lSaveGameInstance = Cast<UCustomSaveGame>(
            UGameplayStatics::LoadGameFromSlot(Constants::Save::kSaveFileName(), 0));

        if (lSaveGameInstance)
        {
            scoreDatas_ = lSaveGameInstance->SavedScores;
        }
    }
}

void UEndScreen::ShowLeaderboard()
{
    leaderboardPanel_->SetVisibility(ESlateVisibility::Visible);
    initialsTextBox_->SetVisibility(ESlateVisibility::Collapsed);
    saveButton_->SetVisibility(ESlateVisibility::Collapsed);
    cancelButton_->SetVisibility(ESlateVisibility::Collapsed);

    scoreScrollBox_->ClearChildren();

    for (const FScoreData& lData : scoreDatas_)
    {
        UTextBlock* lScoreLine = NewObject<UTextBlock>(this);
		lScoreLine->SetColorAndOpacity(FLinearColor::White);
		lScoreLine->SetJustification(ETextJustify::Center);
		lScoreLine->Font.Size = 50;
        lScoreLine->SetText(FText::FromString(lData.ToString()));
        scoreScrollBox_->AddChild(lScoreLine);
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