#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreen.generated.h"

struct FScoreData;

/**
 * End screen showing the final scores
 */
UCLASS()
class OVERBOARD_API UEndScreen : public UUserWidget
{
	GENERATED_BODY()
	
private:
    /**
    * Save file name
    */
    FString const SAVE_KEY = "OverboardSave";
    
    /**
	* Text box to enter the player initials
    */
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* _initialsTextBox;

    /**
	* Panel containing the scores scroll box
    */
    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* _leaderboardPanel;

    /**
	* Scroll box containing the scores
    */
    UPROPERTY(meta = (BindWidget))
    class UScrollBox* _scoreScrollBox;

    /**
	* Button to save the current score
    */
    UPROPERTY(meta = (BindWidget))
    class UButton* _saveButton;

    /**
	* Button to cancel saving and go directlu to the scores panel
    */
    UPROPERTY(meta = (BindWidget))
    class UButton* _cancelButton;

    /**
	* Button to restart the game
    */
    UPROPERTY(meta = (BindWidget))
    class UButton* _restartButton;

    /**
	* Button to quit the game
    */
    UPROPERTY(meta = (BindWidget))
    class UButton* _quitButton;

    /**
	* All scores saved
    */
    TArray<FScoreData> _scoreDatas;

    /**
	* Current player score
    */
    int _score;

private:
    /**
	* Called after the underlying slate widget is constructed.  Depending on how the slate object is used
    */
    virtual void NativeConstruct() override;

    /**
	* Called when the save button is clicked, save the current score
    */
    UFUNCTION()
    void OnSaveClicked();

    /**
	* Called when the text in the initials text box is changed, enable/disable the save button, and force the text to 3 characters
    * 
	* @param pText The new text
    */
    UFUNCTION()
	void OnInitialTextChanged(const FText& pText);

    /**
	* Called when the cancel button is clicked, show the leaderboard panel
    */
    UFUNCTION()
    void OnCancelClicked();

    /**
	* Called when the restart button is clicked, restart the game
    */
    UFUNCTION()
    void OnRestartClicked();

    /**
	* Called when the quit button is clicked, quit the game
    */
    UFUNCTION()
    void OnQuitClicked();

    /**
	* Show the leaderboard panel
    */
    void ShowLeaderboard();

    /**
	* Load the scores from the save file
    */
    void LoadScores();

    /**
	* Save the current score into the save file
    */
    void SaveScoreToFile();
public:
    /**
	* Initialize the end screen with the current score
    */
    void Init(int pScore);
};
