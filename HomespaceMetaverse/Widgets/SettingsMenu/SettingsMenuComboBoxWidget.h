#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuComboBoxWidget.generated.h"

class UComboBoxString;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingsMenuComboBoxValueChangedEvent);

UCLASS()
class HOMESPACEMETAVERSE_API USettingsMenuComboBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitComboBox(const TArray<FText>& Options, int InitialIndex, const FText& InOptionNameText);
	void SetSelectedIndex(int NewIndex);
	int GetSelectedIndex() const;
	UComboBoxString* GetComboBox();
	void UpdateTexts();

	FOnSettingsMenuComboBoxValueChangedEvent OnValueChanged;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION() void OnSelectionChanged(FString Name, ESelectInfo::Type Type);

	UPROPERTY(Transient) TArray<FText> ComboBoxOptions;

	UPROPERTY(meta = (BindWidget)) UComboBoxString* ComboBox;
	UPROPERTY(meta = (BindWidget)) UTextBlock* OptionNameText;
	UPROPERTY(meta = (BindWidget)) UImage* ArrowImage;
};
