#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/2DSoundClip.h"
#include "MainMenuButtonBaseWidget.generated.h"

class UHSMainMenuWidget;
class UWidgetSwitcher;
class USizeBox;
class UTextBlock;
class UButton;

UCLASS()
class HOMESPACEMETAVERSE_API UMainMenuButtonBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMainMenuWidget(UUserWidget* Widget) { MainMenuWidget = Widget; }

	void SetButtonIndex(int Index) { ButtonIndex = Index; }
	virtual void SetActiveState(bool bIsActive);
	UButton* GetButton() { return Button; }
	FText GetCurrentText();

	UFUNCTION(BlueprintCallable) void SetButtonText(FText Text);

	//UPROPERTY(EditDefaultsOnly) bool bCanBeActive;

	UPROPERTY(EditDefaultsOnly) FLinearColor ActiveTextColor;
	UPROPERTY(EditDefaultsOnly) FLinearColor HoveredTextColor;
	UPROPERTY(EditDefaultsOnly) FLinearColor InactiveTextColor;

	UPROPERTY(EditDefaultsOnly) F2DSoundClip HoverSound;
	UPROPERTY(EditDefaultsOnly) F2DSoundClip ClickSound;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(Transient)
	UUserWidget* MainMenuWidget;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ButtonStateSwitcher;
	UPROPERTY(meta = (BindWidget))
	UButton* Button;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* ButtonText;

	UPROPERTY(Transient) bool bIsButtonActive = false;
	UPROPERTY(Transient) int ButtonIndex = 0;

	UFUNCTION() virtual void OnButtonClicked();
	UFUNCTION() virtual void OnButtonHovered();
	UFUNCTION() virtual void OnButtonUnhovered();
	UFUNCTION() virtual void OnButtonPressed();
	UFUNCTION() virtual void OnButtonReleased();

	bool bUiInitialized = false;
};
