#pragma once

#include "CoreMinimal.h"
#include "Widgets/MainMenu/Buttons/MainMenuButtonBaseWidget.h"
#include "SettingsMenuTabButtonWidget.generated.h"

class USettingsMenuWidget;

UCLASS()
class HOMESPACEMETAVERSE_API USettingsMenuTabButtonWidget : public UMainMenuButtonBaseWidget
{
	GENERATED_BODY()

public:
	void SetSettingsMenuWidget(USettingsMenuWidget* Widget) { SettingsMenuWidget = Widget; }

protected:
	void OnButtonClicked();

private:
	UPROPERTY(Transient) USettingsMenuWidget* SettingsMenuWidget;
	
};
