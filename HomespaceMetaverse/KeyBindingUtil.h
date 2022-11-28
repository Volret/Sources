#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerInput.h"
#include "Runtime/UMG/Public/UMG.h"
#include "KeyBindingUtil.generated.h"

USTRUCT(BlueprintType)
struct FInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	FString ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	FKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	FString KeyAsString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	uint32 bShift : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	uint32 bCtrl : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	uint32 bAlt : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	uint32 bCmd : 1;

	FInputAction() {}
	FInputAction(const FString InActionName, const bool bInShift, const bool bInCtrl, const bool bInAlt, const bool bInCmd, const FKey InKey)
		: Key(InKey)
		, KeyAsString(Key.GetDisplayName().ToString())
		, bShift(bInShift)
		, bCtrl(bInCtrl)
		, bAlt(bInAlt)
		, bCmd(bInCmd)
	{
		ActionName = InActionName;
	}

	FInputAction(const FInputActionKeyMapping& Action)
		: Key(Action.Key)
		, KeyAsString(Action.Key.GetDisplayName().ToString())
		, bShift(Action.bShift)
		, bCtrl(Action.bCtrl)
		, bAlt(Action.bAlt)
		, bCmd(Action.bCmd)
	{
		ActionName = Action.ActionName.ToString();
	}

	bool operator!=(const FInputAction& Other)
	{
		return ActionName != Other.ActionName || Key != Other.Key || bShift != Other.bShift || bCtrl != Other.bCtrl || bAlt != Other.bAlt || bCmd != Other.bCmd;
	}
	bool operator==(const FInputAction& Other)
	{
		return ActionName == Other.ActionName && Key == Other.Key && bShift == Other.bShift && bCtrl == Other.bCtrl && bAlt == Other.bAlt && bCmd == Other.bCmd;
	}
};

USTRUCT(BlueprintType)
struct FInputAxis
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	FString AxisName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	FString KeyAsString = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	FKey Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Bindings")
	float Scale = 1;

	FInputAxis() {}
	FInputAxis(const FString InAxisName,float InScale, FKey InKey)
		: AxisName(InAxisName)
		, KeyAsString(InKey.GetDisplayName().ToString())
		, Key(InKey)
		, Scale(InScale)
	{ }

	FInputAxis(const FInputAxisKeyMapping& Axis)
		: AxisName(Axis.AxisName.ToString())
		, KeyAsString(Axis.Key.GetDisplayName().ToString())
		, Key(Axis.Key)		
		, Scale(Axis.Scale)
	{
	}

	bool operator!=(const FInputAxis& Other)
	{
		return AxisName != Other.AxisName || Key != Other.Key || Scale != Other.Scale;
	}
	bool operator==(const FInputAxis& Other)
	{
		return AxisName == Other.AxisName && Key == Other.Key && Scale == Other.Scale;
	}
};

/**
 * 
 */
UCLASS()
class UKeyBindingUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static FInputAxis GetInputAxisFromMouseButtonEvent(const FPointerEvent& MouseEvent);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static FInputAxis GetInputAxisFromMouseAxisEvent(const FPointerEvent& MouseEvent);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static FInputAction GetInputActionFromMouseButtonEvent(const FPointerEvent& MouseEvent);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static FInputAxis GetInputAxisFromKeyEvent(const FKeyEvent& KeyEvent);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static FInputAction GetInputActionFromKeyEvent(const FKeyEvent& KeyEvent);
	
	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static void GetAllBindedInputActions(TArray<FInputAction>& Actions);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static void GetAllBindedInputAxis(TArray<FInputAxis>& InputAxis);

	static bool HasAxisBinding(const FInputAxis& Binding);
	static bool HasAxisBinding(const FString& AxisName, float Scale);
	static TArray<FInputAxis> GetAxisBindings(const FString& ActionName, float Scale);
	static FInputAxis GetAxisBinding(const FString& ActionName, float Scale, const FKey& Key);

	static bool HasActionBinding(const FInputAction& Binding);
	static bool HasActionBinding(const FString& ActionName);

	static bool HasAxisBoundToKey(const TArray<FString>& AxisNames, const FKey& Key);
	static bool HasActionBoundToKey(const TArray<FString>& ActionNames, const FKey& Key);

	static TArray<FInputAction> GetActionBindings(const FString& ActionName);
	static FInputAction GetActionBinding(const FString& ActionName, const FKey& Key);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool ReBindAxisKey(FInputAxis CurrentBinding, FInputAxis NewBinding);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool ReBindActionKey(FInputAction CurrentBinding, FInputAction NewBinding);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool AddAxisBinding(FString AxisName, FInputAxis NewBinding);
	
	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool AddActionBinding(FString ActionName, FInputAction NewBinding);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool RemoveAxisBinding(FInputAxis BindingToRemove);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool RemoveAxisBindingsByName(const FString& AxisName, float Scale);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool RemoveActionBinding(FInputAction BindingToRemove);

	UFUNCTION(BlueprintPure, Category = "Key Bindings")
	static bool RemoveActionBindingsByName(const FString& ActionName);

	static bool ApplyBindingUpdates();
};
