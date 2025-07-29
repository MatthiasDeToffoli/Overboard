// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreData.h"
#include "EndScreen.generated.h"

/**
 * 
 */
UCLASS()
class OVERBOARD_API UEndScreen : public UUserWidget
{
	GENERATED_BODY()
	
private:
    FString const SAVE_KEY = "Save";
    // UI Bindings
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* _initialsTextBox;

    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* _leaderboardPanel;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* _scoreScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* _saveButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* _cancelButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* _restartButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* _quitButton;

    TArray<ScoreData> _scoreDatas;

    int _score;

private:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnSaveClicked();

    UFUNCTION()
    void OnCancelClicked();

    UFUNCTION()
    void OnRestartClicked();

    UFUNCTION()
    void OnQuitClicked();

    void ShowLeaderboard();

    void LoadScores();
    void SaveScoreToFile(const FString& pName);
public:
    void Init(int pScore);
};
