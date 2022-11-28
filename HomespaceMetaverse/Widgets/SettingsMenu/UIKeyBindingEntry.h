#pragma once

#include "CoreMinimal.h"

#include "../../KeyBindingUtil.h"
#include "Blueprint/UserWidget.h"

#include "UIKeyBindingEntry.generated.h"

class USettingsMenuWidget;
class UWidgetSwitcher;
class UCanvasPanel;
class UTextBlock;
class UButton;

UCLASS()
class HOMESPACEMETAVERSE_API UUIKeyBindingEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FText& InActionDisplayName, const FString& ActionName, const FInputAction& InInputActionOne, const FInputAction& InInputActionTwo);
	void Init(const FText& InAxisDisplayName, const FString& AxisName, const FInputAxis& InInputAxisOne, const FInputAxis& InInputAxisTwo);
	void SetSettingsMenuWidget(USettingsMenuWidget* Widget) { SettingsMenuWidget = Widget; }
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void UpdateBindingText();
	FString AxisToString(const FInputAxis& InputAxis);
	FString ActionToString(const FInputAction& InputAction);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void ClearFocus() { bIsWainingForInput = false; }
	void RestoreDefaults();

private:
	UPROPERTY(Transient) USettingsMenuWidget* SettingsMenuWidget;

	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher* ButtonOneSwitcher;
	UPROPERTY(meta = (BindWidget)) UWidgetSwitcher* ButtonTwoSwitcher;

	UPROPERTY(meta = (BindWidget)) UTextBlock* TextAction;
	UPROPERTY(meta = (BindWidget)) UButton* ButtonBindingOne;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBindingOne;
	UPROPERTY(meta = (BindWidget)) UButton* ButtonBindingTwo;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextBindingTwo;
	UPROPERTY(meta = (BindWidget)) UCanvasPanel* PanelInputHandler;

	FText DisplayAction;
	FText DisplayAxis;
	FString Axis;
	FString Action;
	
	FInputAction InputActionOne;
	FInputAxis InputAxisOne;
	
	FInputAction InputActionTwo;
	FInputAxis InputAxisTwo;
	
	bool bIsWainingForInput = false;
	int WaitingInputIndex = 0;

	bool bIsAxis = false;
	float AxisScale = 1;
	
	UFUNCTION()
	void OnClickOne();
	UFUNCTION()
	void OnClickTwo();
	
	void Init();

	bool ActionAlreadyBoundToThisKey(const FString& ActionName, const FKey& Key);
	bool AxisAlreadyBoundToThisKey(const FString& AxisName, const FKey& Key);
};
